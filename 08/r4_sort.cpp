#include <vector>
#include <algorithm>
#include <cassert>

/* Implement an in-place selection sort of a vector of integers,
 * using a «comparator» passed to the sort routine as an argument.
 * A comparator is a function that is used to compare elements
 * instead of the builtin relational operators. This is useful if
 * your data is sorted in non-standard manner. */

void selectsort( std::vector< int > &to_sort, auto cmp );

int main()
{
    using vec = std::vector< int >;
    vec a{ 3, 5, 2, 1, 4 },
        b{ 1, 3, 2, 4 },
        c{ 51, 9, 3, 7, 1 },
        d{ 1, 9, 2, 8 };

    auto a_cmp = []( int a, int b ) { return a > b; };
    auto b_cmp = []( int a, int b ) { return a < b; };
    auto c_cmp = []( int a, int b )
    {
        return std::pair( a % 7, a / 7 ) < std::pair( b % 7, b / 7 );
    };
    auto d_cmp = []( int a, int b ) { return a % 7 < b % 7; };

    selectsort( a, a_cmp );
    selectsort( b, b_cmp );
    selectsort( c, c_cmp );
    selectsort( d, d_cmp );

    assert( std::is_sorted( a.begin(), a.end(), a_cmp ) );
    assert( std::is_sorted( b.begin(), b.end(), b_cmp ) );
    assert( std::is_sorted( c.begin(), c.end(), c_cmp ) );
    assert( std::is_sorted( d.begin(), d.end(), d_cmp ) );

    return 0;
}
