#include <set>
#include <utility>
#include <cassert>

/* Build a reflexive closure of a relation given as a set of pairs,
 * returning the result. */

using relation = std::set< std::pair< int, int > >;

relation reflexive( const relation &r );

int main()
{
    relation r_1{ { 1, 2 }, { 2, 3 } },
             r_2{ { 1, 1 }, { 2, 2 }, { 3, 3 }, { 1, 2 }, { 2, 3 } },
             r_3{ { 1, 1 }, { 1, 2 } },
             r_4{ { 1, 1 }, { 1, 2 }, { 2, 2 } };

    assert( reflexive( r_1 ) == r_2 );
    assert( reflexive( r_2 ) == r_2 );
    assert( reflexive( r_3 ) == r_4 );
    assert( reflexive( r_4 ) == r_4 );

    return 0;
}
