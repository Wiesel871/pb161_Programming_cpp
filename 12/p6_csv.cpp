#include <sstream>
#include <cassert>

/* In this exercise, we will deal with CSV files: we will implement
 * a class called ‹csv› which will read data from an input stream
 * and allow the user to access it using the indexing operator. */

/* The exception to throw in case of format error. */

class bad_format;

/* The constructor should accept a reference to ‹std::istream› and
 * the expected number of columns. In the input, each line contains
 * integers separated by value. The constructor should throw an
 * instance of ‹bad_format› if the number of columns does not match.
 *
 * Additionally, if ‹x› is an instance of ‹csv›, then ‹x.at( 3, 1 )›
 * should return the value in the third row and first column. */

class csv;

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
