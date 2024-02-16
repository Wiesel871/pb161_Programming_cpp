#include <vector>
#include <cassert>

/* Implementujte generický podprogram ‹weighted_sort( seq₁, seq₂,
 * weight )› kde:
 *
 *  1. ‹seq₁› a ‹seq₂› jsou dva stejně dlouhé vektory prvků blíže
 *     neurčeného typu (nemusí být možné je kopírovat),
 *  2. ‹weight› je funkce, která jako parametry přijme jeden prvek
 *     ze ‹seq₁› a jeden prvek ze ‹seq₂› a přiřadí jim celočíselnou
 *     váhu.
 *
 * Vaším úkolem je paralelně přeuspořádat obě sekvence ‹seq₁›,
 * ‹seq₂› tak, aby na konci pro každý platný index ‹i ≥ 1› platilo:
 *
 *     weight( seq₁[ i - 1 ], seq₂[ i - 1 ] ) <=
 *     weight( seq₁[ i ], seq₂[ i ] )
 *
 * Paralelnost přeuspořádání v tomto případě znamená, že:
 *
 *  • byl-li původní prvek ‹seq₁[ i ]› přesunut na novou pozici
 *    ‹seq₁[ j ]›,
 *  • byl zároveň prvek ‹seq₂[ i ]› přesunut na pozici ‹seq₂[ j ]›.
 */

int main()
{
    std::vector a{ 3, 4, 0 },
                b{ 3, 1, 1 };

    weighted_sort( a, b, []( int x, int y ) { return x + y; } );
    assert(( a == std::vector{ 0, 4, 3 } ));
    assert(( b == std::vector{ 1, 1, 3 } ));

    weighted_sort( a, b, []( int x, int y ) { return x - y; } );
    assert(( a == std::vector{ 0, 3, 4 } ));
    assert(( b == std::vector{ 1, 3, 1 } ));

    weighted_sort( a, b, []( int x, int y ) { return y - x; } );
    assert(( a == std::vector{ 4, 3, 0 } ));
    assert(( b == std::vector{ 1, 3, 1 } ));

    return 0;
}
