#include <cassert>
#include <tuple>

/* Implement a binary search tree, i.e. a binary tree which maintains
 * the search property. That is, a value of each node is:
 *
 *  • ≥ than all values in its left subtree,
 *  • ≤ than all values in its right subtree.
 *
 * Store the nodes in a pool (a vector or a list, your choice). The
 * interface is as follows:
 *
 *  • ‹node_ref root() const› returns the root node,
 *  • ‹bool empty() const› checks whether the tree is empty,
 *  • ‹void insert( int v )› inserts a new value into the tree
 *    (without rebalancing).
 *
 * The ‹node_ref› class then ought to provide:
 *
 *  • ‹node_ref left() const› and ‹node_ref right() const›,
 *  • ‹bool valid() const›,
 *  • ‹value() const› which returns the value stored in the node.
 *
 * Calling ‹root› on an empty tree is undefined. */

struct node; /* ref:  6 lines */

using node_pool = std::vector< node >;

class node_ref; /* ref: 12 lines */
class tree;     /* ref: 28 lines */

int main()
{
    struct test
    {
        static std::tuple< bool, int, int > verify( node_ref n, int bound )
        {
            if ( !n.valid() )
                return { true, bound, bound };

            auto l = n.left(), r = n.right();
            auto [ l_ok, l_min, l_max ] = verify( l, n.value() );
            auto [ r_ok, r_min, r_max ] = verify( r, n.value() );

            bool ok = l_ok && r_ok &&
                      l_max <= n.value() && n.value() <= r_min;

            return { ok, l_min, r_max };
        }

        static bool has( node_ref n, int v )
        {
            if ( v == n.value() )
                return true;

            if ( v < n.value() )
                return has( n.left(), v );
            else
                return has( n.right(), v );
        }
    };

    tree t;

    auto insert_check = [&]( int k )
    {
        t.insert( k );
        auto [ ok, min, max ] = test::verify( t.root(), 0 );
        return ok && test::has( t.root(), k );
    };

    assert( insert_check( 7 ) );
    assert( insert_check( 8 ) );
    assert( insert_check( 3 ) );
    assert( insert_check( 4 ) );
    assert( insert_check( 5 ) );
    assert( insert_check( 9 ) );

    assert( test::has( t.root(), 7 ) );
    assert( test::has( t.root(), 3 ) );

    return 0;
}
