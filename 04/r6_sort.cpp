#include <vector>
#include <list>
#include <cassert>
#include <random>
#include <algorithm>

/* Implement ‹sort› which works both on vectors (‹std::vector›) and
 * linked lists (‹std::list›) of integers. The former should use
 * in-place quicksort, while the latter should use merge sort (it's
 * okay to use the ‹splice› and ‹merge› methods on lists, but not
 * ‹sort›). Feel free to refer back to ‹01/r5› for the quicksort. */

int main()
{
    std::vector< int >
        va{ 1, 2 },
        vb{ 2, 1 },
        vc{ 3, 2, 4, 1 },
        vd{ 3, 3, 1, 1, 2 },
        ve{ 1, 2, 3, 4, 6, 5, 7, -7, 1, 3, 1, 4, 5, 0, 9, -1, 0 };

    std::list< int >
        la{ 1, 2 },
        lb{ 2, 1 },
        lc, ld, le;

    sort( va ); sort( la );
    sort( vb ); sort( lb );

    assert( std::is_sorted( va.begin(), va.end() ) );
    assert( std::is_sorted( la.begin(), la.end() ) );
    assert( std::is_sorted( vb.begin(), vb.end() ) );
    assert( std::is_sorted( lb.begin(), lb.end() ) );

    std::mt19937 rand;

    for ( int i = 0; i < 100; ++ i )
    {
        lc.clear(); ld.clear(); le.clear();

        std::copy( vc.begin(), vc.end(), std::back_inserter( lc ) );
        std::copy( vd.begin(), vd.end(), std::back_inserter( ld ) );
        std::copy( ve.begin(), ve.end(), std::back_inserter( le ) );

        sort( vc ); sort( lc );
        sort( vd ); sort( ld );
        sort( ve ); sort( le );

        assert( std::is_sorted( vc.begin(), vc.end() ) );
        assert( std::is_sorted( vd.begin(), vd.end() ) );
        assert( std::is_sorted( ve.begin(), ve.end() ) );

        assert( std::is_sorted( lc.begin(), lc.end() ) );
        assert( std::is_sorted( ld.begin(), ld.end() ) );
        assert( std::is_sorted( le.begin(), le.end() ) );

        std::shuffle( vc.begin(), vc.end(), rand );
        std::shuffle( vd.begin(), vd.end(), rand );
        std::shuffle( ve.begin(), ve.end(), rand );
    }

    return 0;
}
