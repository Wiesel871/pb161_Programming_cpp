#include <cassert>

/* Implement a linked list of integers, with ‹head›, ‹tail› (returns
 * a reference) and ‹empty›. Asking for a ‹head› or ‹tail› of an
 * empty list has undefined results. A default-constructed list is
 * empty. The other constructor takes an int (the value of head) and
 * a reference to an existing list. It will should make a copy of
 * the latter. */

class list;

int main()
{
    list nil,
         l_1( 1, nil ), l_2( 2, l_1 ),
         l_3( 7, l_2 ), l_4( 5, l_2 );

    assert(  nil.empty() );
    assert( !l_1.empty() );
    assert(  l_1.head() == 1 );
    assert(  l_1.tail().empty() );

    assert( !l_2.empty() );
    assert(  l_2.head() == 2 );
    assert( !l_2.tail().empty() );
    assert(  l_2.tail().head() == 1 );
    assert(  l_2.tail().tail().empty() );

    assert( !l_3.empty() );
    assert(  l_3.head() == 7 );
    assert(  l_3.tail().head() == 2 );
    assert( !l_3.tail().tail().empty() );
    assert(  l_3.tail().tail().head() == 1 );
    assert(  l_3.tail().tail().tail().empty() );

    assert( !l_4.empty() );
    assert(  l_4.head() == 5 );
    assert(  l_4.tail().head() == 2 );
    assert( !l_4.tail().tail().empty() );
    assert(  l_4.tail().tail().head() == 1 );
    assert(  l_4.tail().tail().tail().empty() );

    return 0;
}
