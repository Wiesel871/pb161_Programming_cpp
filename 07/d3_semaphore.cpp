#include <cassert>
#include <stdexcept>
#include <utility>

/* In this demo, we will implement a simple semaphore. A semaphore
 * is a device which guards a resource of which there are multiple
 * instances, but the number of instances is limited. It is a slight
 * generalization of a mutex (which guards a singleton resource).
 * Internally, semaphore simply counts the number of clients who
 * hold the resource and refuses further requests if the maximum is
 * reached. In a multi-threaded program, semaphores would typically
 * block (wait for a slot to become available) instead of refusing.
 * In a single-threaded program (which is what we are going to use
 * for a demonstration), this would not work. Hence our ‹get› method
 * returns a ‹bool›, indicating whether acquisition of the lock
 * succeeded. */

class semaphore
{
    int _available;
public:

    /* When a semaphore is constructed, we need to know how many
     * instances of the resource are available. */

    explicit semaphore( int max ) : _available( max ) {}

    /* Classes which represent resource managers (in this case
     * ‘things that can be locked’ as opposed to ‘locks held’) have
     * some tough choices to make. If they are impossible to
     * copy/move/assign, users will find that they must not appear
     * as attributes in their classes, lest those too become
     * un-copyable (and un-movable) by default. However, this is how
     * the standard library deals with the problem, see ‹std::mutex›
     * or ‹std::condition_variable›. While it is the safest option,
     * it is also the most annoying. Nonetheless, we will do the
     * same. */

    semaphore( const semaphore & ) = delete;
    semaphore &operator=( const semaphore & ) = delete;

    /* We allow would-be lock holders to query the number of
     * resource instances currently available. Perhaps if none are
     * left, they can make do without one, or they can perform some
     * other activity in the hopes that the resource becomes
     * available later. */

    int available() const
    {
        return _available;
    }

    /* Finally, what follows is the ‘low-level’ interface to the
     * semaphore. It is completely unsafe, and it is inadvisable to
     * use it directly, other than perhaps in special circumstances.
     * This being C++, such interfaces are commonly made available.
     * Again see ‹std::mutex› for an example.
     *
     * However, it would also be an option to be strict about it,
     * make the following 2 methods private, and declare the RAII
     * class defined below, ‹semaphore_lock›, to be a friend of this
     * one. */

    bool get()
    {
        if ( _available > 0 )
            return _available --;
        else
            return false;
    }

    void put()
    {
        ++ _available;
    }
};

/* We will want to write a RAII ‘lock holder’ class. However, since
 * ‹get› above might fail, we need a way to indicate the failure in
 * the RAII class as well. But constructors don't return values: it
 * is therefore a reasonable choice to throw an exception. It is
 * reasonable as long as we don't expect the failure to be a common
 * scenario. */

class resource_exhausted : public std::runtime_error
{
public:
    resource_exhausted()
        : std::runtime_error( "semaphore full" )
    {}
};

/* Now the RAII class itself. It will need to hold a reference to
 * the semaphore for which it holds a lock (good thing the semaphore
 * is not movable, so we don't have to think about its address
 * changing). Of course, it must not be possible to make a copy of
 * the resource class: we cannot duplicate the resource, which is a
 * lock being held. However, it does make sense to move the lock to
 * a new owner, if the client so wishes. Hence, both a move
 * constructor and move assignment are appropriate. */

class semaphore_lock
{
    semaphore *_sem = nullptr;
public:

    /* To construct a semaphore lock, we understandably need a
     * reference to the semaphore which we wish to lock. You might
     * be wondering why the attribute is a pointer and the argument
     * is a reference. The main difference between references and
     * pointers (except the syntactic sugar) is that references
     * cannot be null. In a correct program, all references always
     * refer to valid objects. It does not make sense to construct a
     * semaphore_lock which does not lock anything. Hence the
     * reference. Why the pointer in the attributes? That will
     * become clear shortly.
     *
     * Before we move on, notice that, as promised, we throw an
     * exception if the locking fails. Hence, no ‹noexcept› on this
     * constructor. */

    semaphore_lock( semaphore &s ) : _sem( &s )
    {
        if ( !_sem->get() )
            throw resource_exhausted();
    }

    /* As outlined above, semaphore locks cannot be copied or
     * assigned. Let's make that explicit. */

    semaphore_lock( const semaphore_lock & ) = delete;
    semaphore_lock &operator=( const semaphore_lock & ) = delete;

    /* The new object (the one initialized by the move constructor)
     * is quite unremarkable. The interesting part is what happens
     * to the ‘old’ (source) instance: we need to make sure that
     * when it is destroyed, it does not release the resource (i.e.
     * the lock held) – the ownership of that has been transferred
     * to the new instance. This is where the pointer comes in
     * handy: we can assign ‹nullptr› to the pointer held by the
     * source instance. Then we just need to be careful when we
     * release the resource (in the destructor, but also in the move
     * assignment operator) – we must first check whether the
     * pointer is valid.
     *
     * Also notice the ‹noexcept› qualifier: even though the
     * ‘normal’ constructor throws, we are not trying to obtain a
     * new resource here, and there is nothing in the constructor
     * that might fail. This is good, because move constructors, as
     * a general rule, should not throw. */

    semaphore_lock( semaphore_lock &&src ) noexcept
        : _sem( src._sem )
    {
        src._sem = nullptr;
    }

    /* We now define a helper method, ‹release›, which frees up
     * (releases) the resource held by this instance. It will do
     * this by calling ‹put› on the semaphore. However, if the
     * semaphore is null, we do nothing: the instance has been moved
     * from, and no longer owns any resources.
     *
     * Why the helper method? Two reasons:
     *
     *  1. it will be useful in both the move assignment operator and
     *     in the destructor,
     *  2. the client might need to release the resource before the
     *     instance goes out of scope or is otherwise destroyed
     *     ‘naturally’ (compare ‹std::fstream::close()›). */

    void release() noexcept
    {
        if ( _sem )
            _sem->put();
    }

    /* Armed with ‹release›, writing both the move assignment and
     * the destructor is easy. The move assignment is also
     * ‹noexcept›, which is usually a pretty good idea. */

    semaphore_lock &operator=( semaphore_lock &&src ) noexcept
    {
        /* Self-move is not very useful in this case, forbid it. */

        assert( &src != this );

        /* First release the resource held by the current instance.
         * We cannot hold both the old and the new resource at the
         * same time. */

        release();

        /* Now we reset our ‹_sem› pointer and update the ‹src›
         * instance – the resource is now in our ownership. */

        _sem = src._sem;
        src._sem = nullptr;
        return *this;
    }

    ~semaphore_lock() noexcept
    {
        release();
    }
};

int main() /* demo */
{
    semaphore sem( 3 );
    sem.get();
    semaphore_lock l1( sem );
    bool l4_made = false;

    try
    {
        semaphore_lock l2( sem );
        assert( sem.available() == 0 );
        semaphore_lock l3 = std::move( l2 );
        assert( sem.available() == 0 );
        semaphore_lock l4 = std::move( l1 );
        assert( sem.available() == 0 );
        l4_made = true;
        semaphore_lock l5( sem );
        assert( false );
    }
    catch ( const resource_exhausted & ) {}

    assert( l4_made );
    assert( sem.available() == 2 );

    // clang-tidy: -clang-analyzer-deadcode.DeadStores
}
