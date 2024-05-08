#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <regex>
#include <iostream>

/* To practice working with IO streams a little, we will write a two
 * simple functions which reads lines from an input stream, process
 * them a little and possibly print them out or their part into an
 * output stream. */

/* The ‹grep› function checks, for every line on the input, whether
 * it matches a given ‹pattern› (i.e. the pattern is a substring of
 * the line) and if it does (and only if it does) copies the line to
 * the output stream. */
struct line : std::string { 
    friend std::istream & operator>>(std::istream &is, line &line) {   
        return std::getline(is, line);
    }
};

void grep( std::string pattern, std::istream &is, std::ostream &os) {
    line line;
    while (is >> line)
        if (line.find(pattern) != line.npos)
            os << line << '\n';
};

/* The other function to add is called ‹cut› and it will process the
 * lines differently: it splits each line into fields separated by
 * the character ‹delim› and only prints the column given by ‹col›.
 * Unlike the ‹cut› program, index columns starting at 0. If there
 * are not enough columns on a given line, print an empty line. */

void cut( char delim, int col, std::istream &is, std::ostream &os) {
    line line;
    while (is >> line) {
        int cur_col = 0;
        auto st = line.begin();
        auto prev = st;
        while ((st = std::find(st, line.end(), delim)) != line.end()) {
            if (cur_col == col)
                break;
            ++st;
            prev = st;
            ++cur_col;
        }
        if (cur_col == col) {
            os << std::string(prev, st);
        }
        os << '\n';
    }
};

int main()
{
    const char *grep_in = "hello\nworld\nbye\nworld\n";
    const char *cut_in  = "a,b|c,d\ne|f|g,h\ni,j,k|l\n";

    auto check_grep = [&]( std::string pat, std::string expect )
    {
        std::istringstream istr( grep_in );
        std::ostringstream ostr;
        grep( pat, istr, ostr );
        assert( ostr.str() == expect );
    };

    check_grep( "o", "hello\nworld\nworld\n" );
    check_grep( "hell", "hello\n" );
    check_grep( "world", "world\nworld\n" );

    auto check_cut = [&]( char d, int c, std::string expect )
    {
        std::istringstream istr( cut_in );
        std::ostringstream ostr;
        cut( d, c, istr, ostr );
        assert( ostr.str() == expect );
    };

    check_cut( ',', 0, "a\ne|f|g\ni\n" );
    check_cut( ',', 1, "b|c\nh\nj\n" );
    check_cut( ',', 2, "d\n\nk|l\n" );
    check_cut( '|', 0, "a,b\ne\ni,j,k\n" );
    check_cut( '|', 1, "c,d\nf\nl\n" );
    check_cut( '|', 2, "\ng,h\n\n" );

    return 0;
}
