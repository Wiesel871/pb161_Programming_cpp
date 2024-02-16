#include <vector>
#include <cassert>

/* Filter out duplicate entries from a vector, maintaining the
 * relative order of entries. Return the result as a new vector. */

std::vector< int > unique( const std::vector< int > & );

int main()
{
    std::vector< int >  in_1{ 1, 2, 4, 3, 1, 7, 3, 5, 1 },
                       out_1{ 1, 2, 4, 3,    7,    5    },
                        in_2{ 1, 2, 3 },
                        in_3{ 1, 1, 1 },
                       out_3{ 1 };

    assert( unique( in_1 ) == out_1 );
    assert( unique( in_2 ) == in_2 );
    assert( unique( in_3 ) == out_3 );

    return 0;
}
