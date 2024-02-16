#include <map>
#include <cassert>

/* † Budeme opět pracovat s orientovaným grafem – tentokrát budeme
 * hledat cykly. Existuje na výběr několik algoritmů, ty založené na
 * prohledávání do hloubky jsou nejjednodušší. Graf je zadaný jako
 * hodnota typu ‹std::multimap› – více se o této reprezentaci
 * dozvíte v ukázce ‹d5_bfs›.
 *
 * Čistá funkce ‹is_dag› nechť vrátí ‹false› právě když ‹g› obsahuje
 * cyklus. Pozor, graf nemusí být souvislý. */

using graph = std::multimap< int, int >;
bool is_dag( const graph &g );

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
