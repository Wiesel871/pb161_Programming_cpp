#include <cassert>
#include <cmath>
#include <utility>
#include <string>

/* Ve cvičení ‹07/p6_linear› jsme napsali jednoduchý program, který
 * řeší systémy lineárních rovnic o dvou neznámých. Tento program
 * nyní rozšíříme o načítání vstupu z řetězce. */

/* Naprogramujte čistou funkci ‹solve›, která má jediný parametr
 * (řetězec předaný jako hodnota typu ‹std::string_view›). Vstup
 * obsahuje právě dvě rovnice, na každém řádku jedna, se dvěma
 * jednopísmennými proměnnými a celočíselnými koeficienty. Každý
 * člen je oddělen od operátorů (‹+› a ‹-›) a znaku rovnosti
 * mezerami, jednotlivý člen (koeficient včetně znaménka a případná
 * proměnná) naopak mezery neobsahuje. Není-li vstup v očekávaném
 * formátu, situaci řešte jak uznáte za vhodné (můžete např. ukončit
 * funkci výjimkou ‹no_parse›).
 *
 * Výsledkem bude dvojice čísel typu ‹double›. Pořadí výsledku nechť
 * je abecední (např. dvojice ‹x›, ‹y›). Jinak se funkce ‹solve›
 * chová stejně, jak je popsáno v zmiňovaném příkladu
 * ‹07/p6_linear›. */

struct no_solution : std::exception {};
struct no_parse    : std::exception {};

std::pair< double, double > solve( std::string_view eq );

int main()
{
    std::pair a( 1.0, 0.0 ), b( -1.0, 0.0 ), c( .6, -.2 );

    auto check = []( auto s, auto val )
    {
        auto [ u, v ] = solve( s );
        auto [ x, y ] = val;
        return std::abs( u - x ) < 1e-6 &&
               std::abs( v - y ) < 1e-6;
    };

    assert( check( " x + y     =  1\n x -  y     =  1\n", a ) );
    assert( check( " x + y     = -1\n x -  y     = -1\n", b ) );
    assert( check( " x + y     =  x\n x +  y     =  1\n", a ) );
    assert( check( "2x + y     =  1\n x - 2y     =  1\n", c ) );
    assert( check( " x + y - 1 =  0\n x -  y - 1 =  0\n", a ) );
    assert( check( "10x + 10y  = 10\n x -  y     =  1\n", a ) );

    try
    {
        solve( "x + y = 1\nx + y = 2\n" );
        assert( false );
    }
    catch ( const no_solution & ) {}

    return 0;
}
