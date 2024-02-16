#include <cassert>
#include <stdexcept>
#include <new>

/* Exceptions are, as their name suggests, a mechanism for handling
 * unexpected or otherwise «exceptional» circumstances, typically
 * error conditions. A canonic example would be trying to open a
 * file which does not exist, trying to allocate memory when there
 * is no free memory left and the like. Another common circumstance
 * would be errors during processing user input: bad format,
 * unexpected switches and so on.
 *
 * NB. Do «not» use exceptions for ‘normal’ control flow, e.g.  for
 * terminating loops. That is a «really» bad idea (even though ‹try›
 * blocks are cheap, throwing exceptions is very expensive). */

/* This example will be somewhat banal. We start by creating a class
 * which has a global counter of instances attached to it: i.e. the
 * value of ‹counter› tells us how many instances of ‹counted› exist
 * at any given time. Fair warning, do not do this at home. */

int counter = 0;

struct counted
{
    counted()  { ++ counter; }
    ~counted() { -- counter; }

    counted( const counted & ) = delete;
    counted( counted && ) = delete;
    counted &operator=( const counted & ) = delete;
    counted &operator=( counted && ) = delete;
};

/* A few functions which throw exceptions and/or create instances of
 * the ‹counted› class above. Notice that a ‹throw› statement
 * immediately stops the execution and propagates up the call stack
 * until it hits a ‹try› block (shown in the ‹main› function below).
 * The same applies to a function call which hits an exception: the
 * calling function is interrupted immediately. */

int f() { counted x; return 7; }
int g() { counted x; throw std::bad_alloc(); assert( 0 ); }
int h() { throw std::runtime_error( "h" ); }
int i() { counted x; g(); assert( 0 ); }

int main() /* demo */
{
    bool caught = false;

    /* A ‹try› block allows us to detect that an exception was
     * thrown and react, based on the type and attributes of the
     * exception. Otherwise, it is a regular block with associated
     * scope, and behaves normally. */

    try
    {
        counted x;
        assert( counter == 1 );
        f();
        assert( counter == 1 );
    }

    /* One or more ‹catch› blocks can be attached to a ‹try› block:
     * those describe what to do in case an exception of a matching
     * type is thrown in one of the statements of the ‹try› block.
     * The ‹catch› clause behaves like a prototype of a
     * single-argument function -- if it could be ‘called’ with the
     * thrown exception as an argument, it is executed to «handle»
     * the exception. */

    /* This particular ‹catch› block is never executed, because
     * nothing in the associated ‹try› block above throws a matching
     * exception (or rather, any exception at all): */

    catch ( const std::bad_alloc & ) { assert( false ); }

    /* The ‹counted› instance ‹x› above went out of scope: */

    assert( counter == 0 );

    /* Let's write another ‹try› block. This time, the ‹i› call in
     * the ‹try› block throws, indirectly (via ‹g›) an exception of
     * type ‹std::bad_alloc›. */

    try { i(); }

    /* To demonstrate how ‹catch› blocks are selected, we will first
     * add one for ‹std::runtime_error›, which will not trigger (the
     * ‘prototype’ does not match the exception type that was
     * thrown): */

    catch ( const std::runtime_error & ) { assert( false ); }

    /* As mentioned above, each ‹try› block can have multiple
     * ‹catch› blocks, so let's add another one, this time for the
     * ‹bad_alloc› that is actually thrown. If the ‹catch› matches
     * the exception type, it is executed and propagation of the
     * exception is stopped: it is now handled and execution
     * continues normally after the end of the ‹catch› sequence. */

    catch ( const std::bad_alloc & ) { caught = true; }

    /* Execution continues here. We check that the ‹catch› block
     * was actually executed: */

    assert( caught );
    assert( counter == 0 ); // no ‹counted› instances were leaked
}
