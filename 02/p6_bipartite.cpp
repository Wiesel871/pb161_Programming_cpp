#include <map>
#include <queue>
#include <vector>
#include <cassert>
#include <queue>


/* Rozhodněte, zda je zadaný neorientovaný graf bipartitní (tzn.
 * jeho vrcholy lze rozdělit do dvou množin ⟦A, B⟧ tak, že každá
 * hrana má jeden vrchol v ⟦A⟧ a jeden v ⟦B⟧). Protože graf je
 * neorientovaný, seznamy sousedů na vstupu jsou symetrické. */

using edges = std::vector< int >;
using graph = std::map< int, edges >;

enum color {
    white = 0,
    black = 1,
};

using colors = std::map<int, color>;

bool is_bipartite( const graph &g ) {
    if (g.empty())
        return true;
    std::queue<int> q;
    colors c = {{g.begin()->first, white}};
    q.emplace(g.begin()->first);
    for (const auto &x: g) {
        if (c.count(x.first) == 0)
            q.emplace(x.first);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            // no idea why but "c.contains" doesnt work on set int int even tho it 
            // worked in p5 *
            color next = c[v] == black ? white : black;
            const auto &e = g.at(v);
            for (const auto &n: e) {
                // * here too
                if (c.count(n) == 0) {
                    c[n] = next;
                    q.push(n);
                    continue;
                }
                if (c[n] == c[v])
                    return false;
            }
        }

    }
    return true;
}

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
