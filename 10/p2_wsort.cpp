#include <cstddef>
#include <utility>
#include <vector>
#include <cassert>
#include <ranges>
#include <numeric>
#include <functional>
#include <algorithm>
#include <concepts>
#include <iterator>



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
template<typename Container>
concept Iterable = requires(Container a) {
    { a.size() } -> std::convertible_to<std::size_t>;
    { a.begin() } -> std::random_access_iterator;
    { a.end() } -> std::random_access_iterator;
};

template <Iterable It1, Iterable It2>
void weighted_sort(It1 &seq1, It2 &seq2, auto weight) {
    // jak sem hledal rieseni tak sem dosel na takyto (nevyuzity) zazrak:
    // using weight_type = decltype(weight(std::declval<typename decltype(seq1)::value_type>(), std::declval<typename  decltype(seq2)::value_type>()));
    std::vector<std::size_t> res_i;
    for (std::size_t i = 0; i < seq1.size(); ++i)
        res_i.push_back(i);
    std::sort(res_i.begin(), res_i.end(),
            [&](std::size_t i, std::size_t j) {
                return weight(seq1[i], seq2[i]) < weight(seq1[j], seq2[j]);
            });
    It1 res1;
    It2 res2;
    for (std::size_t i: res_i) {
        res1.push_back(seq1[i]);
        res2.push_back(seq2[i]);
    }
    seq1 = std::move(res1);
    seq2 = std::move(res2);
}

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
