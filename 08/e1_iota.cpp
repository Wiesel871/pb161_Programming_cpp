#include <cassert>

/* Implement a generic function ‹iota›, which, given a function ‹f›,
 * calls ‹f( start )›, ‹f( start + 1 )›, … ‹f( end - 1 )›, in this
 * order. */

// void iota( … f, int start, int end );

int main()
{
    int i = 0;
    iota( [&]( int j ) { assert( i++ == j - 7); }, 7, 12 );
    assert( i == 5 );

    return 0;
}
