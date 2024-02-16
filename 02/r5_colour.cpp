#include <set>
#include <utility>
#include <cassert>

/* Write a function to decide whether a given graph can be
 * 3-colored. A correct colouring is an assignment of colours to
 * vertices such that no edge connects vertices with the same
 * colour. The graph is given as a set of edges. Edges are
 * represented as pairs; assume that if ⟦(u, v)⟧ is a part of the
 * graph, so is ⟦(v, u)⟧. */

using graph = std::set< std::pair< int, int > >;
bool has_3colouring( const graph &g );

int main()
{
    graph g{ { 1, 2 }, { 2, 1 } };
    graph h{ { 9, 1 }, { 9, 2 }, { 9, 3 }, { 9, 4 },
             { 1, 9 }, { 2, 9 }, { 3, 9 }, { 4, 9 },
             { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 }, { 2, 4 },
             { 2, 1 }, { 3, 2 }, { 4, 3 }, { 1, 4 }, { 4, 2 } };
    graph i{ { 1, 2 }, { 1, 3 }, { 2, 3 },
             { 2, 1 }, { 3, 1 }, { 3, 2 } };
    graph j{ { 1, 2 }, { 1, 3 }, { 1, 4 }, { 2, 3 }, { 2, 4 }, { 3, 4 },
             { 2, 1 }, { 3, 1 }, { 4, 1 }, { 3, 2 }, { 4, 2 }, { 4, 3 } };
    graph k{ { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 },
             { 2, 1 }, { 3, 2 }, { 4, 3 }, { 1, 4 } };
    graph l{ { 1, 2 }, { 3, 4 }, { 4, 5 }, { 3, 5 },
             { 2, 1 }, { 4, 3 }, { 5, 4 }, { 5, 3 } };

    assert(  has_3colouring( g ) );
    assert( !has_3colouring( h ) );
    assert(  has_3colouring( i ) );
    assert( !has_3colouring( j ) );
    assert(  has_3colouring( k ) );
    assert(  has_3colouring( l ) );

    return 0;
}
