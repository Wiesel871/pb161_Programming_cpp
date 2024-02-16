#include <set>
#include <vector>
#include <cassert>

/* Rozložte zadaný neorientovaný graf na souvislé komponenty
 * (výsledné komponenty budou reprezentované množinou svých
 * vrcholů). Graf je zadaný jako symetrická matice sousednosti.
 * Vrcholy jsou očíslované od 1 do ⟦n⟧, kde ⟦n⟧ je velikost vstupní
 * matice.
 *
 * V grafu je hrana ⟦{u, v}⟧ přítomna právě tehdy, je-li na řádku
 * ⟦u⟧ ve sloupci ⟦v⟧ hodnota ‹true›. */

using graph = std::vector< std::vector< bool > >;

using component = std::set< int >;
using components = std::set< component >;

components decompose( const graph &g );

int main()
{
    graph g_1{ { 0, 1, 1, 0 },
               { 1, 0, 0, 0 },
               { 1, 0, 0, 0 },
               { 0, 0, 0, 0 } },

          g_2{ { 0, 1, 0, 0 },
               { 1, 0, 1, 0 },
               { 0, 1, 0, 1 },
               { 0, 0, 1, 0 } },

          g_3{ { 0, 1, 0, 0 },
               { 1, 0, 0, 0 },
               { 0, 0, 0, 1 },
               { 0, 0, 1, 0 } },

          g_4{ { 0, 0, 0, 0 },
               { 0, 0, 0, 1 },
               { 0, 0, 0, 0 },
               { 0, 1, 0, 0 } },

          g_5{ { 0, 0, 0, 0 },
               { 0, 0, 0, 0 },
               { 0, 0, 0, 0 },
               { 0, 0, 0, 0 } },

          g_6{ { 0, 1, 1, 0 },
               { 1, 0, 1, 0 },
               { 1, 1, 0, 0 },
               { 0, 0, 0, 0 } },

          g_7{ { 0, 1, 1, 1 },
               { 1, 0, 1, 1 },
               { 1, 1, 0, 1 },
               { 1, 1, 1, 0 } };

    components c_1{ { 1, 2, 3 }, { 4 } },
               c_2{ { 1, 2, 3, 4 } },
               c_3{ { 1, 2 }, { 3, 4 } },
               c_4{ { 1 }, { 2, 4 }, { 3 } },
               c_5{ { 1 }, { 2 }, { 3 }, { 4 } },
               c_6{ { 1, 2, 3 }, { 4 } },
               c_7{ { 1, 2, 3, 4 } };

    assert( decompose( g_1 ) == c_1 );
    assert( decompose( g_2 ) == c_2 );
    assert( decompose( g_3 ) == c_3 );
    assert( decompose( g_4 ) == c_4 );
    assert( decompose( g_5 ) == c_5 );
    assert( decompose( g_6 ) == c_6 );
    assert( decompose( g_7 ) == c_7 );

    return 0;

    // clang-tidy: -modernize-use-bool-literals
}
