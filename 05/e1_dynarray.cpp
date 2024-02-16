#include <cassert>

/* Implement a dynamic array of integers with 2 operations: element
 * access (using methods ‹get( i )› and ‹set( i, v )›) and
 * ‹resize( n )›. The constructor takes the initial size as its only
 * parameter. */

struct dynarray;

int main()
{
    dynarray d( 3 );

    d.set( 0, 1 );
    d.set( 1, 2 );
    d.set( 2, 3 );
    assert( d.get( 0 ) == 1 );
    assert( d.get( 2 ) == 3 );

    d.resize( 2 );
    assert( d.get( 0 ) == 1 );
    assert( d.get( 1 ) == 2 );

    d.resize( 4 );
    assert( d.get( 0 ) == 1 );
    assert( d.get( 1 ) == 2 );

    d.set( 2, 3 );
    d.set( 3, 4 );
    assert( d.get( 0 ) == 1 );
    assert( d.get( 1 ) == 2 );
    assert( d.get( 2 ) == 3 );
    assert( d.get( 3 ) == 4 );

    return 0;
}
