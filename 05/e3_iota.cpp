#include <cassert>

/* Write a class ‹iota›, which can be iterated using a ‹range› for
 * to yield a sequence of numbers in the range ‹start›, ‹end - 1›
 * passed to the constructor. */

class iota;

int main()
{
    int i = 0;
    for ( int j : iota( 7, 13 ) )
        assert( 7 + i++ == j );
    assert( i == 6 );

    return 0;
}
