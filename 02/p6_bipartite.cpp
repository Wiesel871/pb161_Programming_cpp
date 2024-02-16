#include <map>
#include <vector>
#include <cassert>

/* Rozhodněte, zda je zadaný neorientovaný graf bipartitní (tzn.
 * jeho vrcholy lze rozdělit do dvou množin ⟦A, B⟧ tak, že každá
 * hrana má jeden vrchol v ⟦A⟧ a jeden v ⟦B⟧). Protože graf je
 * neorientovaný, seznamy sousedů na vstupu jsou symetrické. */

using edges = std::vector< int >;
using graph = std::map< int, edges >;

bool is_bipartite( const graph &g );

int main()
{
    assert(  is_bipartite( graph() ) );
    assert(  is_bipartite( graph{ { 1, {} } } ) );
    assert(  is_bipartite( graph{ { 1, { 2 } }, { 2, { 1 } } } ) );
    assert(  is_bipartite( graph{ { 1, {} }, { 2, {} } } ) );

    graph g{ { 1, { 2, 3 } }, { 2, { 1, 3 } }, { 3, { 1, 2 } } };
    assert( !is_bipartite( g ) );

    graph h{ { 1, { 2 } }, { 2, { 1, 3 } }, { 3, { 2 } } };
    assert( is_bipartite( h ) );

    graph i{ { 1, { 2, 3 } }, { 2, { 1 } }, { 3, { 1 } } };
    assert( is_bipartite( i ) );

    graph j{ { -1, { -2 } }, { -2, { -1 } },
             { 1, { 2, 3 } }, { 2, { 1, 3 } }, { 3, { 1, 2 } } };
    assert( !is_bipartite( j ) );

    return 0;
}
