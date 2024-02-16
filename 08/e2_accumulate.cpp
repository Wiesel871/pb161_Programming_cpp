#include <vector>
#include <cassert>

/* Write a function ‹accumulate( f, vec )› which will sum up ⟦f(x)⟧
 * for all ⟦x⟧ in the given ‹std::vector< int > vec›. */

// …

int main()
{
    auto f = []( int x ) { return x * x; };
    auto g = []( int x ) { return x / 2; };
    auto h = []( int x ) { return x; };

    std::vector vec{ 1, 2, 3, 4 };

    assert( accumulate( f, vec ) == 30 );
    assert( accumulate( g, vec ) ==  4 );
    assert( accumulate( h, vec ) == 10 );

    return 0;
}
