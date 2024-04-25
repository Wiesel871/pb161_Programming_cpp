#include <cstddef>
#include <vector>
#include <cassert>
#include <ranges>
#include <numeric>
#include <functional>
#include <algorithm>
#include <concepts>
#include <iterator>


/* Napište čistou, generickou funkci ‹distinct( s )›, která spočítá,
 * kolik různých prvků se objevuje ve vzestupně seřazené
 * posloupnosti ‹s›. Zadaná posloupnost je hodnota typu, který
 * poskytuje metody ‹begin› a ‹end›; výsledné iterátory lze
 * efektivně (v konstantním čase) posouvat o libovolný počet pozic
 * (např. funkcí ‹std::next›) a lze také efektivně získat vzdálenost
 * dvou iterátorů (např. funkcí ‹std::distance›). Prvky nemusí být
 * možné kopírovat, ale lze je libovolně srovnávat a přesouvat.
 *
 * Funkce musí pracovat v čase nejvýše ⟦O(k⋅\log(n))⟧, kde ⟦k⟧ je
 * počet různých prvků (výsledek volání ‹distinct›) a ⟦n⟧ je délka
 * vstupní posloupnosti. */
template<typename T>
concept Iterable = requires(T a) {
    { a.size() } -> std::convertible_to<std::size_t>;
    { a.begin() } -> std::indirectly_readable;
    { a.end() } -> std::indirectly_readable;
    
};

template<Iterable It>
std::size_t distinct(const It &cont) {
    if (cont.begin() == cont.end())
        return 0;
    auto cur = cont.begin();
    std::size_t res = 0;
    while (cur != cont.end()) {
        res++;
        cur = std::upper_bound(cur, cont.end(), *cur);
    }
    return res;
}


int main()
{
    assert(( distinct( std::vector{ 1, 1, 2, 2 } ) == 2 ));
    assert(( distinct( std::vector{ 1, 3, 3, 4 } ) == 3 ));

    struct token
    {
        int value;
        int *ops;

        token( int v, int &ops ) : value( v ), ops( &ops ) {}

        token( const token & ) = delete;
        token( token && ) = default;

        token &operator=( const token & ) = delete;
        token &operator=( token && ) = default;

        bool operator==( const token &o ) const
        {
             ++ *ops;
             return value == o.value;
        }

        auto operator<=>( const token &o ) const
        {
            ++ *ops;
            return value <=> o.value;
        }
    };

    std::vector< token > x;
    int ops = 0;

    for ( int i = 0; i < 1024; ++i )
        x.emplace_back( i / 128, ops );

    assert( distinct( x ) == 8 );
    assert( ops <= 160 );

    return 0;
}
