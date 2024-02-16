#include <vector>
#include <cassert>

/* Napište podprogram ‹reorder( s, weight )›, který pro zadanou
 * posloupnost ‹s› a funkci ‹weight› na místě přeuspořádá ‹s› tak,
 * že pro ⟦u < w⟧ budou prvky s váhou ⟦u⟧ předcházet prvkům s váhou
 * ⟦w⟧. Zároveň pro prvky se stejnou váhou platí, že se objeví ve
 * stejném pořadí, v jakém byly v původní posloupnosti ‹s›.
 *
 * Podprogram musí pracovat v čase nejvýše ⟦O(k⋅n)⟧ kde ⟦k⟧ je počet
 * různých vah, které se objeví na vstupu, a ⟦n⟧ je délka
 * posloupnosti ‹s›. Je také povoleno využít lineární množství
 * dodatečné paměti. */

int main()
{
    std::vector a{ 1, 3, 4, 2 };

    reorder( a, []( int x ) { return x / 2; } );
    assert(( a == std::vector{ 1, 3, 2, 4 } ));
    reorder( a, []( int x ) { return -x / 2; } );
    assert(( a == std::vector{ 4, 3, 2, 1 } ));

    struct token
    {
        int value;

        token( int v ) : value( v ) {}

        token( const token & ) = delete;
        token( token && ) = default;

        token &operator=( const token & ) = delete;
        token &operator=( token && ) = default;

        bool operator==( const token & ) const = default;
    };

    struct weight
    {
        int value;
        weight( int v ) : value( v ) {}

        weight( const weight & ) = delete;
        weight( weight && ) = delete;

        weight &operator=( const weight & ) = delete;
        weight &operator=( weight && ) = delete;

        bool operator==( const weight &o ) const = default;
        auto operator<=>( const weight &o ) const = default;
    };

    std::vector< token > x, y;
    int ops = 0;
    int count = 1024 * 512;

    for ( int i = 0; i < count; ++i )
    {
        x.emplace_back( i );
        y.emplace_back( i );
    }

    auto one_way = [&]( const auto &t )
    {
        ++ops;
        return weight( -t.value / count );
    };

    auto two_way = [&]( const auto &t )
    {
        ++ops;
        return weight( -t.value / ( count / 2 ) );
    };

    reorder( x, one_way );
    assert( ops <= 4 * count + 100 );
    assert( x == y ); /* no change */
    ops = 0;

    reorder( x, two_way );
    assert( ops <= 8 * count + 100 );
    assert( x != y );
    ops = 0;

    assert( x.front().value == count / 2 );
    assert( x.back().value == count / 2 - 1 );
    assert( x[ count / 2 ].value == 0 );
    assert( x[ count / 2 - 1 ].value == count - 1 );

    return 0;
}
