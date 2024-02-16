#include <set>
#include <cassert>

/* Naprogramujte typ, který bude reprezentovat symetrickou binární
 * relaci na celých číslech s těmito metodami:
 *
 *  • ‹add› poznamená, že zadaná čísla jsou v relaci,
 *  • ‹test› ověří, zda jsou zadaná čísla v relaci,
 *  • ‹get› vrátí množinu čísel, která jsou se zadaným v relaci,
 *  • ‹set_filter› nastaví filtr (binární predikát) – pomyslně
 *    z relace odstraní dvojice, které predikát nesplňují,
 *  • ‹unset_filter› zruší nastavený filtr.
 *
 * Pozor, nastavením filtru se nemění sestavená relace, pouze dotazy
 * na ni. Je-li filtr odstraněn, relace se tím vrátí do původního
 * stavu. */

struct relation;

int main()
{
    relation r;
    const auto &const_r = r;

    std::set set_a{ 1, 2, 3 },
             set_b{ 1, 2, 5, 7, 8 };

    for ( int a : set_a )
        for ( int b : set_b )
            if ( a % 3 == b % 3 )
                r.add( a, b );

    for ( int a : set_a )
        for ( int b : set_b )
        {
            bool expect = a % 3 == b % 3;
            assert( const_r.test( a, b ) == expect );
            assert( const_r.test( b, a ) == expect );
        }

    auto is_even = []( int a, int b )
    {
        return a % 2 == 0 && b % 2 == 0;
    };

    assert(( r.get( 1 ) == std::set{ 1, 7 } ));
    assert(( r.get( 7 ) == std::set{ 1 } ));
    assert(( r.get( 2 ) == std::set{ 2, 5, 8 } ));

    r.set_filter( is_even );

    for ( int a : set_a )
        for ( int b : set_b )
        {
            bool expect = a % 3 == b % 3 && a % 2 == 0 && b % 2 == 0;
            assert( const_r.test( a, b ) == expect );
            assert( const_r.test( b, a ) == expect );
        }

    assert( r.get( 3 ).empty() );
    assert(( r.get( 2 ) == std::set{ 2, 8 } ));

    r.unset_filter();
    assert(( r.get( 2 ) == std::set{ 2, 5, 8 } ));

    for ( int a : set_a )
        for ( int b : set_b )
        {
            bool expect = a % 3 == b % 3;
            assert( const_r.test( a, b ) == expect );
            assert( const_r.test( b, a ) == expect );
        }

    return 0;
}
