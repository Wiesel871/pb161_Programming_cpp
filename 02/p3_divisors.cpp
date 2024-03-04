#include <cmath>

#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

/* Nalezněte všechny prvočíselné dělitele čísla ‹num› a vložte je do
 * vektoru ‹divs›. Počáteční hodnota parametru ‹divs›:
 *
 *  • obsahuje právě všechny prvočíselné dělitele všech čísel «ostře
 *    menších» než ‹num›,
 *  • je vzestupně seřazená.
 *
 * «Výstupní» podmínkou pro vektor ‹divs› je:
 *
 *  • obsahuje všechna čísla, která obsahoval na vstupu,
 *  • zároveň obsahuje všechny prvočíselné dělitele čísla ‹num›,
 *  • je vzestupně seřazený.
 *
 * Funkce musí pracovat «efektivně». Určit vhodnou časovou složitost
 * je v této úloze součástí zadání. */

void add_divisors( int num, std::vector< int > &divs ) {
    auto res = std::vector<int>{};
    std::size_t i = 0;
    for (int j = 2; j <= num; j += 1 + (j > 2)) {
        if ((i < divs.size() && j == divs[i])) {
            res.push_back(j);
            i++;
        } else if (num % j == 0) {
            res.push_back(j);
        }
        while (num % j == 0)
            num /= j;
    }
    while (i < divs.size()) {
        res.push_back(divs[i++]);
    }
    divs = res;
}

int main()
{
    std::vector< int > divs{ 2, 3, 5 };

    add_divisors( 7, divs );
    assert( divs.size() == 4 );
    assert( divs.back() == 7 );

    add_divisors( 8, divs );
    assert( divs.size() == 4 );

    add_divisors( 9, divs );
    assert( divs.size() == 4 );

    add_divisors( 10, divs );
    assert( divs.size() == 4 );
    assert( divs[ 0 ] == 2 );
    assert( divs[ 1 ] == 3 );
    assert( divs[ 2 ] == 5 );
    assert( divs[ 3 ] == 7 );

    return 0;
}
