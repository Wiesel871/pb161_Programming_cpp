#include <cassert>

/* Implementujte množinu libovolných celých čísel, s těmito
 * operacemi:
 *
 *  • sjednocení operátorem ‹|›,
 *  • průnik operátorem ‹&›,
 *  • rozdíl operátorem ‹-›,
 *  • uspořádání inkluzí relačními operátory.
 *
 * Všechny výše uvedené operace musí být nejvýše lineární v součtu
 * velikostí vstupních množin. Typ ‹set› doplňte metodami ‹add›
 * (přidá prvek) a ‹has› (ověří přítomnost prvku), které mají
 * nejvýše logaritmickou složitost. */

struct set;

int main()
{
    set a, b;
    a.add( 1 ), a.add( 2 ), a.add( 5 ), a.add( -3 );
    b.add( 1 ), b.add( -3 ), b.add( -1000 ), b.add( 1 << 20 );

    const set x = a, y = b;

    const set u = x | y;
    const set i = x & y;
    const set d = x - y;

    assert(  u.has( 1 ) );
    assert(  i.has( 1 ) );
    assert( !d.has( 1 ) );

    assert(  u.has( 2 ) );
    assert( !i.has( 2 ) );
    assert(  d.has( 2 ) );
    assert( !i.has( -1000 ) );
    assert(  u.has( -1000 ) );
    assert( !d.has( -1000 ) );
    assert(  u.has( 1 << 20 ) );
    assert( !i.has( 1 << 20 ) );

    assert( !( a <= b ) );
    assert( !( b <= a ) );
    assert( a <= u );
    assert( u >  a );
    assert( b <= u );
    assert( i <= u );
    assert( i <  u );
    assert( i <= a );
    assert( i <= b );
    assert( u <= u );
    assert( u == u );
    assert( d <= u );
    assert( d <= a );
    assert( !( d <= b ) );

    return 0;
}
