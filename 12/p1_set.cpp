#include <cassert>
#include <sstream>

/* Implementujte typ ‹set›, který reprezentuje množinu libovolných
 * celých čísel, s operacemi:
 *
 *  • ‹add› – přidá prvek,
 *  • ‹has› – ověří přítomnost prvku,
 *  • ‹size› – vrátí počet prvků.
 *
 * Dále nechť je hodnoty typu ‹set› možné číst z a zapisovat do
 * vstupně-výstupních proudů. Na výstupu budou mít množiny tuto
 * formu:
 *
 *     {}
 *     { 1 }
 *     { 1, 2 }
 *
 * Na vstupu akceptujte i varianty s jiným počtem bílých znaků
 * (včetně žádných). */

struct set;

int main()
{
    // clang-tidy: -readability-container-size-empty

    std::stringstream s( "{3} {} {1} {1 } {1 ,2}" );
    set m;
    const auto &const_m = m;

    s >> m;
    assert( s && m.size() == 1 && m.has( 3 ) );
    s >> m;
    assert( s && const_m.size() == 0 && !m.has( 3 ) );
    s >> m;
    assert( s && m.size() == 1 && m.has( 1 ) );
    s >> m;
    assert( s && m.size() == 1 && const_m.has( 1 ) );
    s >> m;
    assert( s && m.size() == 2 && m.has( 1 ) && m.has( 2 ) );

    set n, o;
    n.add( 1 );

    s.str( "" );
    s.clear();

    s << const_m << " " << n << " " << o;
    assert( s.str() == "{ 1, 2 } { 1 } {}" );

    return 0;
}
