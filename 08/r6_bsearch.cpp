#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

/* Implement binary search on a vector, with a twist: the order of
 * the elements is given by a «comparator». This is a function that
 * is passed as an argument to ‹search› and is used to compare
 * elements instead of the builtin relational operators. This is
 * useful if your data is sorted in non-standard manner. */

// auto search = []( std::vector< int > &vec, int val, auto cmp );

int main()
{
    using vec = std::vector< int >;
    vec a{ 5, 4, 3, 2, 1 },
        b{ 1, 2, 3, 4 },
        c{ 7, 1, 9, 51, 3 },
        d{ 1, 8, 9, 2 };

    auto a_cmp = []( int a, int b ) { return a > b; };
    auto b_cmp = []( int a, int b ) { return a < b; };
    auto c_cmp = []( int a, int b )
    {
        return std::pair( a % 7, a / 7 ) < std::pair( b % 7, b / 7 );
    };
    auto d_cmp = []( int a, int b ) { return a % 7 < b % 7; };

    assert( std::is_sorted( a.begin(), a.end(), a_cmp ) );
    assert( std::is_sorted( b.begin(), b.end(), b_cmp ) );
    assert( std::is_sorted( c.begin(), c.end(), c_cmp ) );
    assert( std::is_sorted( d.begin(), d.end(), d_cmp ) );

    assert( search( a, 1, a_cmp ) == std::next( a.begin(), 4 ) );
    assert( search( a, 2, a_cmp ) == std::next( a.begin(), 3 ) );
    assert( search( a, 3, a_cmp ) == std::next( a.begin(), 2 ) );
    assert( search( a, 4, a_cmp ) == std::next( a.begin(), 1 ) );
    assert( search( a, 5, a_cmp ) == std::next( a.begin(), 0 ) );
    assert( search( a, 0, a_cmp ) == a.end() );
    assert( search( a, 6, a_cmp ) == a.end() );

    assert( search( b, 1, b_cmp ) == std::next( b.begin(), 0 ) );
    assert( search( b, 2, b_cmp ) == std::next( b.begin(), 1 ) );
    assert( search( b, 3, b_cmp ) == std::next( b.begin(), 2 ) );
    assert( search( b, 4, b_cmp ) == std::next( b.begin(), 3 ) );
    assert( search( b, 0, b_cmp ) == b.end() );
    assert( search( b, 5, b_cmp ) == b.end() );

    assert( search( c, 1,  c_cmp )  == std::next( c.begin(), 1 ) );
    assert( search( c, 2,  c_cmp )  == c.end() );
    assert( search( c, 3,  c_cmp )  == std::next( c.begin(), 4 ) );
    assert( search( c, 4,  c_cmp )  == c.end() );
    assert( search( c, 5,  c_cmp )  == c.end() );
    assert( search( c, 7,  c_cmp )  == std::next( c.begin(), 0 ) );
    assert( search( c, 9,  c_cmp )  == std::next( c.begin(), 2 ) );
    assert( search( c, 11, c_cmp ) == c.end() );
    assert( search( c, 51, c_cmp ) == std::next( c.begin(), 3 ) );
    assert( search( c, 0,  c_cmp )  == c.end() );

    assert( search( d,  1,  d_cmp ) != d.end() );
    assert( search( d,  8,  d_cmp ) != d.end() );
    assert( search( d, 15,  d_cmp ) != d.end() );
    assert( search( d,  1,  d_cmp ) == search( d, 8, d_cmp ) );
    assert( search( d, 17,  d_cmp ) == d.end() );

    return 0;
}
