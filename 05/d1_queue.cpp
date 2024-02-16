#include <cassert>
#include <memory>

/* In this example, we will demonstrate the use of
 * ‹std::unique_ptr›, which is an RAII class for holding (owning)
 * values dynamically allocated from the heap. We will implement a
 * simple one-way, non-indexable queue. We will require that it is
 * possible to erase elements from the middle in O(1), without
 * invalidating any other iterators. The standard containers
 * which could fit:
 *
 *  • ‹std::deque› fails the erase in the middle requirement,
 *  • ‹std::forward_list› does not directly support queue-like
 *    operation, hence using it as a queue is possible but awkward;
 *    wrapping ‹std::forward_list› would be, however, a viable
 *    approach to this task, too,
 *  • ‹std::list› works well as a queue out of the box, but has
 *    twice the memory overhead of ‹std::forward_list›.
 *
 * As usual, since we do not yet understand templates, we will only
 * implement a queue of integers, but it is not hard to imagine we
 * could generalize to any type of element. */

/* Since we are going for a custom, node-based structure, we will
 * need to first define the class to represent the nodes. For sake
 * of simplicity, we will not encapsulate the attributes. */

struct queue_node
{
    /* We do not want to handle all the memory management ourselves.
     * To rule out the possibility of accidentally introducing
     * memory leaks, we will use ‹std::unique_ptr› to manage
     * allocated memory for us. Whenever a ‹unique_ptr› is
     * destroyed, it will free up any associated memory. An
     * important limitation of ‹unique_ptr› is that each piece of
     * memory managed by a ‹unique_ptr› must have «exactly one»
     * instance of ‹unique_ptr› pointing to it. When this instance
     * is destroyed, the memory is deallocated. */

    std::unique_ptr< queue_node > next;

    /* Besides the structure itself, we of course also need to store
     * the actual data. We will store a single integer per node. */

    int value;
};

/* We will also need to be able to iterate over the queue. For that,
 * we define an iterator, which is really just a slightly
 * generalized pointer (you may remember ‹nibble_ptr› from last
 * week). We need 3 things: pre-increment, dereference and
 * inequality. */

struct queue_iterator
{
    queue_node *node;

    /* The ‹queue› will need to create instances of a
     * ‹queue_iterator›. Let's make that convenient. */

    queue_iterator( queue_node *n ) : node( n ) {}

    /* The pre-increment operator simply shifts the pointer to the
     * ‹next› pointer of the currently active node. */

    queue_iterator &operator++()
    {
        node = node->next.get();
        return *this;
    }

    /* Equality is very simple (we need this because the condition
     * of iteration loops is ‹it != c.end()›, including range ‹for›
     * loops). We could implement ‹!=› directly, but ‹==› is usually
     * more natural, and given ‹==›, the compiler will derive ‹!=›
     * for us automatically. */

    bool operator==( const queue_iterator &o ) const
    {
        return o.node == node;
    }

    /* And finally the dereference operator: this is what will be
     * called when ‹*it› is evaluated. Also notice the
     * ‹const›/non-‹const› overloads (for completeness, it is often
     * preferable to return a ‹const› reference from the ‹const›
     * overload; this depends on the element type).*/

    int &operator*()       { return node->value; }
    int  operator*() const { return node->value; }
};

/* This class represents the queue itself. We will have ‹push› and
 * ‹pop› to add and remove items, ‹empty› to check for emptiness and
 * ‹begin› and ‹end› to implement iteration. */

class queue
{
    /* We will keep the head of the list in another ‹unique_ptr›. An
     * empty queue will be represented by a null head. Also worth
     * noting is that when using a list as a queue, the head is
     * where we remove items. The end of the queue (where we add new
     * items) is represented by a plain pointer because it does not
     * «own» the node (the node is owned by its predecessor). */

    std::unique_ptr< queue_node > first;
    queue_node *last = nullptr;
public:

    /* As mentioned above, adding new items is done at the ‘tail’
     * end of the list. This is quite straightforward: we simply
     * create the node, chain it into the list (using the ‹last›
     * pointer as a shortcut) and point the ‹last› pointer at the
     * newly appended node. We need to handle empty and non-empty
     * lists separately because we chose to represent an empty list
     * using null head, instead of using a dummy node. */

    void push( int v )
    {
        if ( last ) /* non-empty list */
        {
            last->next = std::make_unique< queue_node >();
            last = last->next.get();
        }
        else /* empty list */
        {
            first = std::make_unique< queue_node >();
            last = first.get();
        }

        last->value = v;
    }

    /* Reading off the value from the head is easy enough. However,
     * to remove the corresponding node, we need to be able to point
     * ‹first› at the next item in the queue.
     *
     * Unfortunately, we cannot use normal assignment (because
     * copying ‹unique_ptr› is not allowed). We will have to use an
     * operation that is called «move assignment» and which is
     * written using a helper function in from the standard library,
     * called ‹std::move›.
     *
     * Operations which «move» their operands invalidate the
     * «moved-from» instance. In this case, ‹first->next› is the
     * «moved-from» object and the «move» will turn it into a ‹null›
     * pointer. In any case, the ‹next› pointer which was
     * invalidated was stored in the old ‹head› «node» and by
     * rewriting ‹first›, we lost all pointers to that node. This
     * means two things:
     *
     *  1. the old head's ‹next› pointer, now ‹null›, is no longer
     *     accessible
     *  2. memory allocated to hold the old head node is freed */

    int pop()
    {
        int v = first->value;
        first = std::move( first->next );

        /* Do not forget to update the ‹last› pointer in case we
         * popped the last item. */

        if ( !first ) last = nullptr;
        return v;
    }

    /* The emptiness check is simple enough. */

    bool empty() const { return !last; }

    /* Now the ‹begin› and ‹end› methods. We start iterating from
     * the head (since we have no choice but to iterate in the
     * direction of the ‹next› pointers). The ‹end› method should
     * return a so-called «past-the-end» iterator, i.e. one that
     * comes right after the last real element in the queue. For an
     * empty queue, both ‹begin› and ‹end› should be the same.
     * Conveniently, the ‹next› pointer in the last real node is
     * ‹nullptr›, so we can use that as our end-of-queue sentinel
     * quite naturally. You may want to go back to the pre-increment
     * operator of ‹queue_iterator› just in case. */

    queue_iterator begin() { return { first.get() }; }
    queue_iterator end()   { return { nullptr }; }

    /* And finally, erasing elements. Since this is a singly-linked
     * list, to erase an element, we need an iterator to the element
     * «before» the one we are about to erase. This is not really a
     * problem, because erasing at the head is done by ‹pop›. We use
     * the same «move assignment» construct that we have seen in
     * ‹pop› earlier. */

    void erase_after( queue_iterator i )
    {
        assert( i.node->next );
        i.node->next = std::move( i.node->next->next );
    }
};

int main() /* demo */
{
    /* We start by constructing an (empty) queue and doing some
     * basic operations on it. For now, we only try to insert and
     * remove a single element. */

    queue q;
    assert( q.empty() );
    q.push( 7 );
    assert( !q.empty() );
    assert( q.pop() == 7 );
    assert( q.empty() );

    /* Now that we have emptied the queue again, we add a few more
     * items and try erasing one and iterating over the rest.*/

    q.push( 1 );
    q.push( 2 );
    q.push( 7 );
    q.push( 3 );

    /* We check that erase works as expected. We get an iterator
     * that points to the value ‹2› from above and use it to erase
     * the value ‹7›. */

    queue_iterator i = q.begin();
    ++ i;
    assert( *i == 2 );
    q.erase_after( i );

    /* We can use instances of ‹queue› in range ‹for› loops, because
     * they have ‹begin› and ‹end›, and the types those methods
     * return (i.e. iterators) have dereference, inequality and
     * pre-increment. */

    int x = 1;
    for ( int v : q )
        assert( v == x++ );

    /* That went rather well, let's just check that the order of
     * removal is the same as the order of insertion (first in,
     * first out). This is how queues should behave. */

    assert( q.pop() == 1 );
    assert( q.pop() == 2 );
    assert( q.pop() == 3 );
    assert( q.empty() );
}
