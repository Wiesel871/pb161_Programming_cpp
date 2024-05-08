#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

/* In this exercise, we will deal with CSV files: we will implement
 * a class called ‹csv› which will read data from an input stream
 * and allow the user to access it using the indexing operator. */

/* The exception to throw in case of format error. */

class bad_format {};

/* The constructor should accept a reference to ‹std::istream› and
 * the expected number of columns. In the input, each line contains
 * integers separated by value. The constructor should throw an
 * instance of ‹bad_format› if the number of columns does not match.
 *
 * Additionally, if ‹x› is an instance of ‹csv›, then ‹x.at( 3, 1 )›
 * should return the value in the third row and first column. */
struct line : std::string { 
    friend std::istream & operator>>(std::istream &is, line &line) {   
        return std::getline(is, line);
    }
};

class csv {
    std::vector<std::vector<int>> data;

    void append(auto st, auto end, int &cur_col) {
        if (std::find_if(st, end, [](int c) {return !isdigit(c) && !isspace(c) && c != '\0';}) != end)
            throw bad_format();
        std::string col(st, end);
        data.back().push_back(std::atoi(col.data()));
        ++cur_col;
    }

    public:
    csv(std::istream &is, int cols) {
        line line;
        while (is >> line) {
            data.emplace_back();
            int cur_col = 0;
            auto st = line.begin();
            auto prev = st;
            while ((st = std::find(st, line.end(), ',')) != line.end()) {
                if (std::find_if(prev, st, [](int c) {return !isdigit(c) && !isspace(c) && c != '\0';}) != st)
                    throw bad_format();
                append(prev, st, cur_col);
                ++st;
                prev = st;
            }
            append(prev, st, cur_col);

            if (cols != cur_col)
                throw bad_format();
        }
    }

    int at(std::size_t i, std::size_t j) const {
        return data.at(i).at(j);
    }
};

int main()
{
    std::istringstream istr( "1,2,1\n3,4,5\n" );
    csv x( istr, 3 );
    const auto &const_x = x;

    assert( x.at( 0, 0 ) == 1 );
    assert( x.at( 0, 1 ) == 2 );
    assert( x.at( 0, 2 ) == 1 );

    assert( x.at( 1, 0 ) == 3 );
    assert( x.at( 1, 1 ) == 4 );
    assert( const_x.at( 1, 2 ) == 5 );

    /* Unfortunately, resetting the underlying buffer does not clear
     * flags (such as eof) in the stream. We need to do that
     * manually with a call to ‹clear›. */

    istr.clear();
    istr.str( "1,2,3\n1,2\n" );

    try { csv y( istr, 3 ); assert( false ); }
    catch ( const bad_format & ) {}

    istr.clear();
    istr.str( "1,2,3\n1,2\n" );

    try { csv y( istr, 2 ); assert( false ); }
    catch ( const bad_format & ) {}

    return 0;
}
