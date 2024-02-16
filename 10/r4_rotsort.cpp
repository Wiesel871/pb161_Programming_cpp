#include <cassert>
#include <vector>

/* Napište funkci ‹rotate_sort›, která dostane podobně jako
 * v předchozím cvičení na vstupu kontejner (nebo rozsah ve smyslu
 * ‹std::range›) a kladné číslo ‹n›; vstupní posloupnost
 * pak interpretuje jako obdélníkové pole šířky ‹n›. Můžete
 * předpokládat, že:
 *
 *  • iterátory lze efektivně posouvat o zadaný počet pozic (např.
 *    funkcí ‹std::next›),
 *  • délka vstupní posloupnosti je dělitelná ‹n›.
 *
 * Funkce ověří, je-li možné každý řádek vzestupně seřadit jednou
 * rotací (tzn. existuje-li ‹k› takové, že posunem prvků o ‹k› mod
 * ‹n› doprava nebo doleva vznikne seřazená posloupnost). Je-li to
 * možné, tyto rotace provede a vrátí ‹true›. Jinak vstupní sekvenci
 * nijak nemodifikuje a vrátí ‹false›. Celková složitost nechť je
 * ⟦O(m)⟧ kde ⟦m⟧ je délka vstupní posloupnosti. */

int main()
{
    std::vector a{ 6, 6, 1, 3, 5,
                   1, 3, 4, 4, 5,
                   4, 1, 2, 3, 3 };
    std::vector a_sorted{ 1, 3, 5, 6, 6,
                          1, 3, 4, 4, 5,
                          1, 2, 3, 3, 4 };
    std::vector b{ 1, 3, 4,
                   4, 1, 0 };
    std::vector c{ 3, 5, 1, 3, 4,
                   1, 2, 3, 4, 5 };

    auto b_orig = b, c_orig = c;

    assert(  rotate_sort( a, 5 ) );
    assert( !rotate_sort( b, 3 ) );
    assert( !rotate_sort( c, 5 ) );
    assert( a == a_sorted );
    assert( b == b_orig );
    assert( c == c_orig );

    return 0;
}
