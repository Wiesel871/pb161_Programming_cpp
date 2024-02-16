#include <vector>
#include <cassert>
#include <random>
#include <algorithm>

/* Implementujte typ ‹array›, který reprezentuje pole čísel a bude
 * mít metody:
 *
 *  • ‹get( i )› – vrátí hodnotu na indexu ‹i›,
 *  • ‹append( x )› – vloží hodnotu ‹x› na konec pole,
 *  • ‹partition( p, l, r )› – přeuspořádá část pole v rozsahu
 *    indexů ⟦⟨l, r)⟧ tak, aby hodnoty menší než ‹p› předcházely
 *    hodnotě rovné ‹p› a tato předcházela zbývající hodnoty větší
 *    nebo rovné ‹p› (nejsou-li ‹l› a ‹r› uvedeny, přeuspořádá celé
 *    pole; vstupní podmínkou je, že ‹p› je v uvedeném rozsahu
 *    přítomno),
 *  • ‹sort()› – seřadí pole metodou quicksort (bez dodatečné paměti
 *    mimo místa na zásobníku potřebného pro rekurzi).
 *
 * Algoritmus quicksort pracuje takto:
 *
 *  1. má-li pole žádné nebo 1 prvek, je již seřazené: konec;
 *  2. jinak jeden z prvků vybereme jako «pivot»,
 *  3. přeuspořádáme pole na dvě menší «partice» (viz popis metody
 *     ‹partition› výše),
 *  4. rekurzivně aplikuje algoritmus quicksort na levou a pravou
 *     partici (vynechá přitom hodnoty rovné pivotu).
 *
 * Užitečný invariant: po každé partici jsou prvky rovné vybranému
 * pivotu na pozicích, které budou mít ve výsledném uspořádaném
 * poli.
 *
 * Viz též: https://xkcd.com/1185/ */

struct array;

int main()
{
    array arr;

    arr.append( 1 );
    arr.append( 4 );
    arr.append( 3 );
    arr.append( 2 );
    arr.partition( 2 );

    assert( arr.get( 0 ) < 2 );
    assert( arr.get( 1 ) == 2 );
    assert( arr.get( 2 ) > 2 );
    assert( arr.get( 3 ) > 2 );

    auto quicksort = []( auto &v )
    {
        array a;

        for ( auto x : v )
            a.append( x );
        a.sort();

        for ( unsigned i = 0; i < v.size(); ++i )
            v[ i ] = a.get( i );
    };

    std::vector< int >
        a{ 1, 2 },
        b{ 2, 1 },
        c{ 3, 2, 4, 1 },
        d{ 3, 3, 1, 1, 2 },
        e{ 1, 2, 3, 4, 6, 5, 7, -7, 1, 3, 1, 4, 5, 0, 9, -1, 0 };

    quicksort( a );
    quicksort( b );

    assert( std::is_sorted( a.begin(), a.end() ) );
    assert( std::is_sorted( b.begin(), b.end() ) );

    std::mt19937 rand;

    for ( int i = 0; i < 100; ++ i )
    {
        quicksort( c );
        quicksort( d );
        quicksort( e );

        assert( std::is_sorted( c.begin(), c.end() ) );
        assert( std::is_sorted( d.begin(), d.end() ) );
        assert( std::is_sorted( e.begin(), e.end() ) );

        std::shuffle( c.begin(), c.end(), rand );
        std::shuffle( d.begin(), d.end(), rand );
        std::shuffle( e.begin(), e.end(), rand );
    }

    return 0;
}
