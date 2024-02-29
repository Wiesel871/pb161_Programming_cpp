#include <map>
#include <cassert>
#include <vector>
#include <stack>

/* † Budeme opět pracovat s orientovaným grafem – tentokrát budeme
 * hledat cykly. Existuje na výběr několik algoritmů, ty založené na
 * prohledávání do hloubky jsou nejjednodušší. Graf je zadaný jako
 * hodnota typu ‹std::multimap› – více se o této reprezentaci
 * dozvíte v ukázce ‹d5_bfs›.
 *
 * Čistá funkce ‹is_dag› nechť vrátí ‹false› právě když ‹g› obsahuje
 * cyklus. Pozor, graf nemusí být souvislý. */

using graph = std::multimap< int, int >;

enum color {
    grey,
    black,
};

using colors = std::map<int, color>;

bool is_dag_rec(const graph &g, colors &c, int p) {
    c[p] = grey;
    auto [begin, end] = g.equal_range(p);
    for (; begin != end; begin++) {
        auto [_, next] = *begin;
        if (c.contains(next) && c[next] == grey)
            return false;
        if (!c.contains(next)) {
            if (!is_dag_rec(g, c, next))
                return false;
        }
    }
    c[p] = black;
    return true;
}

bool is_dag( const graph &g ) {
    if (g.empty())
        return true;
    colors c = {};
    return is_dag_rec(g, c, g.begin()->first);
}

int main()
{
    assert(  is_dag( graph{ { 1, 2 } } ) );
    assert( !is_dag( graph{ { 1, 1 } } ) );
    assert(  is_dag( graph{ { 1, 2 }, { 1, 3 }, { 2, 3 } } ) );

    graph g{ { 1, 2 }, { 1, 3 }, { 2, 4 }, { 2, 5 }, { 3, 4 } };
    assert( is_dag( g ) );

    graph h{ { 1, 2 }, { 1, 3 }, { 2, 4 }, { 2, 5 }, { 5, 1 } };
    assert( !is_dag( h ) );

    return 0;
}
