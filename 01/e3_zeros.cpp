#include <cassert>

/* Zapišme nezáporné číslo ‹n› v soustavě o základu ‹base›. Určete
 * kolik (nelevostranných) nul se v tomto zápisu objeví. Do
 * výstupního parametru ‹order› uložte řád nejvyšší z nich. Není-li
 * v zápisu nula žádná, hodnotu ‹order› nijak neměňte. */

int zeros( int n, int base, int &order );

int main()
{
    int order = 3;

    assert( zeros( 0, 3, order ) == 0 );
    assert( order == 3 );
    assert( zeros( 1, 3, order ) == 0 );
    assert( order == 3 );
    assert( zeros( 3, 3, order ) == 1 );
    assert( order == 0 );
    assert( zeros( 4, 3, order ) == 0 );
    assert( order == 0 );
    assert( zeros( 6, 3, order ) == 1 );
    assert( zeros( 100, 10, order ) == 2 );
    assert( zeros( 10000, 10, order ) == 4 );
    assert( order == 3 );
    assert( zeros( 21030, 10, order ) == 2 );
    assert( order == 2 );
    assert( zeros( 10310, 10, order ) == 2 );
    assert( order == 3 );

    return 0;
}
