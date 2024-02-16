#include <set>
#include <utility>
#include <cassert>

/* Hra života je dvourozměrný buněčný automat: buňky tvoří
 * čtvercovou síť a mají dva možné stavy: živá nebo mrtvá.
 * V každé generaci (kroku simulace) spočítáme novou hodnotu pro
 * každou buňku, a to podle těchto pravidel (výsledek závisí na
 * současném stavu buňky a na tom, kolik z jejích 8 sousedů je
 * živých):
 *
 * │  stav │ živí sousedi │ výsledek │
 * ├───────┼──────────────┼──────────┤
 * │  živá │          0–1 │    mrtvá │
 * │  živá │          2–3 │     živá │
 * │  živá │          4–8 │    mrtvá │
 * │┄┄┄┄┄┄┄│┄┄┄┄┄┄┄┄┄┄┄┄┄┄│┄┄┄┄┄┄┄┄┄┄│
 * │ mrtvá │          0–2 │    mrtvá │
 * │ mrtvá │            3 │     živá │
 * │ mrtvá │          4-8 │    mrtvá │
 *
 * Příklad krátké periodické hry:
 *
 *  ┌───┬───┬───┐   ┌───┬───┬───┐   ┌───┬───┬───┐
 *  │   │   │   │   │   │ × │   │   │   │   │   │
 *  ├───┼───┼───┤   ├───┼───┼───┤   ├───┼───┼───┤
 *  │ × │ × │ × │ → │   │ × │   │ → │ × │ × │ × │
 *  ├───┼───┼───┤   ├───┼───┼───┤   ├───┼───┼───┤
 *  │   │   │   │   │   │ × │   │   │   │   │   │
 *  └───┴───┴───┘   └───┴───┴───┘   └───┴───┴───┘
 *
 * Napište funkci, která dostane na vstupu množinu živých buněk a
 * vrátí množinu buněk, které jsou živé po ‹n› generacích. Živé
 * buňky jsou zadané svými souřadnicemi, tzn. jako dvojice ⟦x, y⟧.
 */

using cell = std::pair< int, int >;
using grid = std::set< cell >;

grid life( const grid &, int );

int main()
{
    assert( life( {}, 3 ).empty() );

    assert( life( { { 0, 0 } }, 1 ).empty() );
    grid block = { { 0, 0 }, { 1, 1 }, { 0, 1 }, { 1, 0 } };

    grid blinker_0 = { { 0, -1 }, { 0, 0 }, { 0, 1 } };
    grid blinker_1 = { { -1, 0 }, { 0, 0 }, { 1, 0 } };
    grid glider    = blinker_1;

    glider.emplace( 1, -1 );
    glider.emplace( 0, -2 );

    for ( int i = 0; i < 20; ++ i )
        assert( life( block, i ) == block );

    for ( int i = 0; i < 20; ++ i )
    {
        auto expect = i % 2 == 0 ? blinker_0 : blinker_1;
        assert( life( blinker_0, i ) == expect );
    }

    for ( int i = 0; i < 40; i += 4 )
    {
        grid expect;

        for ( auto [ x, y ] : glider )
            expect.emplace( x + i / 4, y + i / 4 );

        assert( life( glider, i ) == expect );
    }

    return 0;
}
