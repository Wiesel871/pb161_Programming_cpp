#include <vector>
#include <cassert>

/* This demo will be our first serious foray into dealing with
 * object lifetime. In particular, we will want to implement binary
 * trees – clearly, the lifetime of tree nodes must exactly match
 * the lifetime of the tree itself:
 *
 *  • if the nodes were released too early, the program would
 *    perform invalid memory access when traversing the tree,
 *  • if the nodes are not released with the tree, that would
 *    be a memory leak – we keep the nodes around, but cannot
 *    access them.
 *
 * This is an ubiquitous problem, and if you think about it, we have
 * encountered it a lot, but did not have to think about it yet: the
 * items in an ‹std::vector› or ‹std::map› or other containers have
 * the same property: their lifetime must match the lifetime of the
 * instance which «owns them».
 *
 * This is one of the most important differences between C and C++:
 * if we do C++ right, most of the time, we do not need to manage
 * object lifetimes manually. This is achieved through two main
 * mechanisms:
 *
 *  1. pervasive use of «automatic variables», through «value
 *     semantics» – local variables and arguments are «automatically
 *     destroyed» when they go out of scope,
 *  2. «cascading» – whenever an object is destroyed, its attributes
 *     are also destroyed «automatically», and a mechanism is provided
 *     for classes which own additional, non-attribute objects (e.g.
 *     elements in an ‹std::vector›) to automatically destroy them
 *     too (this is achieved by «user-defined destructors»).
 *
 * In general, destroying objects at an appropriate time is the job
 * of the «owner» of the object – whether the owner is a function
 * (this is the case with by-value arguments and local variables) or
 * another object (attributes, elements of a container and so on).
 * Additionally, this happens «transparently» for the user: the
 * compiler takes care of inserting the right calls at the right
 * places to ensure everything is destroyed at the right time.
 *
 * The end result is modular or «composable» resource management –
 * well-behaved objects can be composed into well-behaved composites
 * without any additional glue or boilerplate.
 *
 * To make things easy for now, we will take advantage of existing
 * containers to do resource management for us, which will save us
 * from writing destructors (the proverbial glue, which is boring to
 * write and easy to get wrong). In the next chapter, we will see
 * how we can use «smart pointers» for the same purpose. */

/* We will be keeping the nodes of our binary tree in an
 * ‹std::vector› – this means that each node has an «index» which we
 * can use to refer to that node. In other words, in this demo (and
 * in some of this week's exercises) indices will play the role of
 * pointers. Since 0 is a valid index, we will use -1 to indicate an
 * invalid (‘null’) reference. Besides ‘pointers’ to the left and
 * right child, the node will contain a single integer value. */

struct node
{
    int left = -1, right = -1;
    int value;
};

/* As mentioned earlier, the nodes will be held in a vector: let's
 * give a name to the particular vector type, for convenience: */

using node_pool = std::vector< node >;

/* Working with ‹node› is, however, rather inconvenient: we cannot
 * ‘dereference’ the ‹left›/‹right› ‘pointers’ without going through
 * ‹node_pool›. This makes for verbose code which is unpleasant to
 * both read and to write. But we can do better: let's add a simple
 * wrapper type, which will remember both a reference to the
 * ‹node_pool› and an index of the ‹node› we are interested in:
 * values of this type can then behave like a proper reference to
 * ‹node›: only a value of the ‹node_ref› type is needed to access
 * the node and to walk the tree. */

struct node_ref
{
    node_pool &_pool;
    int _idx;

    /* To make the subsequent code easier to write (and read), we
     * will define a few helper methods: first, a ‹get› method which
     * returns an actual reference to the ‹node› instance that this
     * ‹node_ref› represents. */

    node &get() { return _pool[ _idx ]; }

    /* And a method to construct a new ‹node_ref› using the same
     * pool as this one, but with a new index. */

    node_ref make( int idx ) { return { _pool, idx }; }

    /* Normally, we do not want to expose the ‹_pool› or ‹node› to
     * users directly, hence we keep them private. But it's
     * convenient for ‹tree› itself to be able to access them. So we
     * make ‹tree› a friend. */

    node_ref( node_pool &p, int i ) : _pool( p ), _idx( i ) {}

    /* For simplicity, we allow invalid references to be
     * constructed: those will have an index -1, and will naturally
     * arise when we encounter a node with a missing child – that
     * missing node is represented as index -1. The ‹valid› method
     * allows the user to check whether the reference is valid. The
     * remaining methods (‹left›, ‹right› and ‹value›) must not be
     * called on an invalid ‹node_ref›. This is the moral equivalent
     * of a null pointer. */

    bool valid() const { return _idx >= 0; }

    /* What follows is a simple interface for traversing and
     * inspecting the tree. Notice that ‹left› and ‹right› again
     * return ‹node_ref› instances. This makes tree traversal simple
     * and convenient. */

    node_ref left()  { return make( get().left ); }
    node_ref right() { return make( get().right ); }

    int &value() { return get().value; }
};

/* Finally the class to represent the tree as a whole. It will own
 * the nodes (by keeping a ‹node_pool› of them as an attribute, will
 * remember a «root node» (which may be invalid, if the tree is
 * empty) and provide an interface for adding nodes to the tree.
 * Notice that «removal» of nodes is conspicuously missing: that's
 * because the pool model is not well suited for removals (smart
 * pointers will be better in that regard). */

struct tree
{
    node_pool _pool;
    int _root_idx = -1;

    /* A helper method to append a new ‹node› to the pool and return
     * its index. */

    int make( int value )
    {
        _pool.emplace_back();
        _pool.back().value = value;
        return _pool.size() - 1;
    }

    node_ref root() { return { _pool, _root_idx }; }
    bool empty() const { return _root_idx == -1; }

    /* We will use a vector to specify a location in the tree for
     * adding a node, with values -1 (left) and 1 (right). An empty
     * vector represents at the root node. */

    using path_t = std::vector< int >;

    /* Find the location for adding a node recursively and create
     * the node when the location is found. Assumes that the path is
     * correct. */

    void add( node_ref parent, path_t path, int value,
              unsigned path_idx = 0 )
    {
        assert( path_idx < path.size() );
        int dir = path[ path_idx ];

        if ( path_idx < path.size() - 1 )
        {
            auto next = dir < 0 ? parent.left() : parent.right();
            return add( next, path, value, path_idx + 1 );
        }

        if ( dir < 0 )
            parent.get().left = make( value );
        else
            parent.get().right = make( value );
    }

    /* Main entry point for adding nodes. */

    void add( path_t path, int value )
    {
        if ( root().valid() )
            add( root(), path, value );
        else
        {
            assert( path.empty() );
            _root_idx = make( value );
        }
    }
};

int main() /* demo */
{
    tree t;
    t.add( {}, 1 );

    assert( t.root().value() == 1 );
    assert( t.root().valid() );
    assert( !t.root().left().valid() );

    t.add( { -1 }, 7 );
    assert( t.root().value() == 1 );
    assert( t.root().left().valid() );
    assert( t.root().left().value() == 7 );

    t.add( { -1, 1 }, 3 );
    assert( t.root().left().right().value() == 3 );
}
