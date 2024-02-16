#include <vector>
#include <cassert>

/* Find the mode (most common value) in a non-empty vector and
 * return it. If there are more than one, return the smallest. */

int mode( const std::vector< int > & );

int main()
{
    std::vector< int > in_1{ 1, 2, 1, 3, 1, 4, 1, 3, 1, 2, 1 },
                       in_2{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7 },
                       in_3{ 1 };

    assert( mode( in_1 ) == 1 );
    assert( mode( in_2 ) == 7 );
    assert( mode( in_3 ) == 1 );

    return 0;
}
