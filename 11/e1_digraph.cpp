#include <cassert>

/* We will write a simple function, ‹digraph_freq›, which accepts a
 * string and computes the frequency of all (alphabetic) digraphs.
 * The exact signature is up to you, in particular the return type.
 * The only requirement is that the returned value can be indexed
 * using strings and this returns the count (or 0 if the input
 * string is not a correct digraph). This must also work on
 * ‹const› instances of the return value. For examples see ‹main›.
 */

/* Define ‹digraph_freq› here, along with any helper functions or
 * classes. */

int main()
{
    const auto f = digraph_freq( "lorem ipsum dolor" );
    assert( f[ "or" ] == 2 );
    assert( f[ "lo" ] == 2 );
    assert( f[ "ip" ] == 1 );
    assert( f[ "re" ] == 1 );

    assert( f[ "oe" ] == 0 );
    assert( f[ "m " ] == 0 );  /* not an alphabetic digraph */
    assert( f[ "m" ] == 0 );   /* not a digraph either */
    assert( f[ "ore" ] == 0 ); /* still no */
    assert( f[ "mi" ] == 0 );

    return 0;
}
