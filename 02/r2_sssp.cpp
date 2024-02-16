#include <map>
#include <vector>
#include <cassert>

/* Compute single-source shortest path distances for all vertices in
 * an unweighted directed graph. The graph is given using adjacency
 * (successor) lists. The result is a map from a vertex to its
 * shortest distance from ‹initial›. Vertices which are not
 * reachable from ‹initial› should not appear in the result. */

using edges = std::vector< int >;
using graph = std::map< int, edges >;

std::map< int, int > shortest( const graph &g, int initial );

int main()
{
    graph g_1{ { 1, { 2, 3 } }, { 2, { 1, 3 } }, { 3, { 1, 2 } } },
          g_2{ { 1, { 2, 3 } }, { 2, { 4, 5 } }, { 3, { 4 } },
               { 4, {} }, { 5, {} } },
          g_3{ { 1, { 2, 3 } }, { 2, { 4, 5 } }, { 5, { 1 } },
               { 3, {} }, { 4, {} } };

    std::map< int, int > d_1_1{ { 1, 0 }, { 2, 1 }, { 3, 1 } },
                         d_1_2{ { 2, 0 }, { 1, 1 }, { 3, 1 } },
                         d_1_3{ { 3, 0 }, { 1, 1 }, { 2, 1 } },
                         d_2_1{ { 1, 0 }, { 2, 1 }, { 3, 1 }, { 4, 2 }, { 5, 2 } },
                         d_2_2{ { 2, 0 }, { 4, 1 }, { 5, 1 } },
                         d_2_3{ { 3, 0 }, { 4, 1 } },
                         d_2_4{ { 4, 0 } },
                         d_3_1{ { 1, 0 }, { 2, 1 }, { 3, 1 }, { 4, 2 }, { 5, 2 } },
                         d_3_2{ { 2, 0 }, { 4, 1 }, { 5, 1 }, { 1, 2 }, { 3, 3 } },
                         d_3_3{ { 3, 0 } },
                         d_3_4{ { 4, 0 } },
                         d_3_5{ { 5, 0 }, { 1, 1 }, { 2, 2 }, { 3, 2 }, { 4, 3 } };

    assert( shortest( g_1, 1 ) == d_1_1 );
    assert( shortest( g_1, 2 ) == d_1_2 );
    assert( shortest( g_1, 3 ) == d_1_3 );
    assert( shortest( g_2, 1 ) == d_2_1 );
    assert( shortest( g_2, 2 ) == d_2_2 );
    assert( shortest( g_2, 3 ) == d_2_3 );
    assert( shortest( g_2, 4 ) == d_2_4 );
    assert( shortest( g_3, 1 ) == d_3_1 );
    assert( shortest( g_3, 2 ) == d_3_2 );
    assert( shortest( g_3, 3 ) == d_3_3 );
    assert( shortest( g_3, 4 ) == d_3_4 );
    assert( shortest( g_3, 5 ) == d_3_5 );

    return 0;
}
