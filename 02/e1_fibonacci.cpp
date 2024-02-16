#include <vector>
#include <cassert>

/* Fill in an existing vector with a Fibonacci sequence (i.e. after
 * calling ‹fibonacci( v, n )›, the vector ‹v› should contain the
 * first ‹n› Fibonacci numbers, and nothing else). */

// void fibonacci( … )

int main()
{
    std::vector< int > fib, expect{ 1, 1, 2, 3, 5 };

    fibonacci( fib, 5 );
    assert( fib == expect );

    fibonacci( fib, 6 );
    expect.push_back( 8 );
    assert( fib == expect );

    fib = std::vector< int >{ 4, 5 };
    fibonacci( fib, 6 );
    assert( fib == expect );

    return 0;
}
