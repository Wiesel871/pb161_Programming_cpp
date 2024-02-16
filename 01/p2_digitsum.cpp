#include <cassert>

/* Funkce ‹digit_sum› sečte cifry nezáporného čísla ‹num› v zápisu
 * o základu ‹base›. Je-li výsledek ve stejném zápisu víceciferný,
 * sečte cifry tohoto výsledku, atd., až je výsledkem jediná cifra,
 * kterou vrátí jako svůj výsledek. */

int digit_sum( int num, int base );

int main()
{
    assert( digit_sum( 0b1, 2 ) == 1 );
    assert( digit_sum( 0b11, 2 ) == 1 );
    assert( digit_sum( 0b111, 2 ) == 1 );
    assert( digit_sum( 731, 10 ) == 2 );
    assert( digit_sum( 777, 8 ) == 7 );

    return 0;
}
