#include <cassert>
#include <vector>
#include <tuple>

/* Napište čistou funkci ‹sorted_ranges›, která na vstupu dostane
 * kontejner (nebo rozsah ve smyslu ‹std::range›) a kladné číslo
 * ‹n›. Vstupní posloupnost interpretujte jako obdélníkové pole
 * šířky ‹n›. Můžete předpokládat:
 *
 *  • iterátory lze efektivně posouvat o zadaný počet pozic (např.
 *    funkcí ‹std::next›),
 *  • délka vstupní posloupnosti je dělitelná ‹n›.
 *
 * Výstupem bude ‹std::vector› dvojic (index, délka), které pro
 * každý řádek vstupu udávají nejdelší uspořádanou posloupnost
 * nacházející se na tomto řádku. Je-li takových posloupností víc,
 * použije se index první z nich. Celková složitost nechť je ⟦O(m)⟧
 * kde ⟦m⟧ je délka vstupní posloupnosti. */

int main()
{
    using sorted_t = std::vector< std::tuple< int, int > >;

    const std::vector a{ 1, 2, 1, 3, 5,
                         1, 3, 4, 4, 5,
                         4, 1, 2, 3, 0 };
    std::vector b{ 1, 2, 1, 3, 1,
                   2, 3, 4, 5, 4,
                   5, 4, 3, 2, 1 };
    std::vector c{ 1, -2, 1, 1, 3, 3,
                   3, -2, 3, 4, 5, 4,
                   -5, 0, 4, 6, 8, 9 };

    assert(( sorted_ranges( a, 5 ) ==
             sorted_t{ { 2, 3 }, { 0, 5 }, { 1, 3 } } ));
    assert(( sorted_ranges( b, 5 ) ==
             sorted_t{ { 0, 2 }, { 0, 4 }, { 0, 1 } } ));
    assert(( sorted_ranges( c, 6 ) ==
             sorted_t{ { 1, 5 }, { 1, 4 }, { 0, 6 } } ));
    assert(( sorted_ranges( c, 3 ) ==
             sorted_t{ { 1, 2 }, { 0, 3 },
                       { 1, 2 }, { 0, 2 },
                       { 0, 3 }, { 0, 3 } } ));

    assert(( sorted_ranges( a, 15 ) == sorted_t{ { 5, 5 } } ));
    assert(( sorted_ranges( b, 15 ) == sorted_t{ { 4, 5 } } ));
    assert(( sorted_ranges( c, 18 ) == sorted_t{ { 1, 6 } } ));

    assert(( sorted_ranges( std::vector< int >{}, 1 ) ==
             sorted_t() ));
    assert(( sorted_ranges( std::vector< int >{}, 5 ) ==
             sorted_t() ));

    return 0;
}
