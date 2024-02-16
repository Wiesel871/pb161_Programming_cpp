#include <cassert>

/* Same basic idea as ‹circuit.cpp›: we model a circuit made of
 * components. Things get a bit more complicated in this version:
 *
 *  • loops are allowed
 *  • parts have 2 inputs and 2 outputs each */

/* The base class, with the following interface:
 *
 *  • ‹read› takes an integer (decides which output) and returns a
 *    boolean,
 *  • ‹connect› takes two integers and a reference to a component
 *    (the first integer defines the input of ‹this› and the second
 *    integer defines the output of the third argument to connect).
 *
 * There is more than one way to resolve loops, some of which require ‹read› to
 * be virtual (that's okay). Please note that each loop «must» have at least
 * one ‹delay› in it (otherwise, behaviour is still undefined). NB. Each
 * component should first read input 0 and then input 1: the ordering will
 * affect the result.  */

class component; /* ref: 30 lines */

/* A ‹delay› is a component that reads out, on both outputs, the
 * value it has obtained on the corresponding input on the previous
 * call to ‹read›. */

class delay; /* ref: 20 lines */

/* A ‹latch› remembers one bit of information (starting at ‹false›):
 *
 *  • if both inputs read ‹false›, the remembered bit remains unchanged,
 *  • if input 0 is ‹false› while input 1 is ‹true› the remembered bit is
 *    set to ‹true›,
 *  • in all other cases, the remembered bit is set to ‹false›.
 *
 * The value on output 0 is the «new value» of the remembered bit:
 * there is no delay. The value on output 1 is the negation of
 * output 0. */

class latch; /* 15 lines */

/* Finally, the ‹cnot› gate, or a «controlled not» gate has the
 * following behaviour:
 *
 *  • output 0 always matches input 0, while
 *  • output 1 is set to:
 *    ◦ input 1 if input 0 is ‹true›
 *    ◦ negation of input 1 if input 0 is ‹false› */

class cnot; /* ref: 11 lines */

int main()
{
    cnot flip;
    delay d;

    /* ‹cnot› with unconnected input 0 is just a ‘not’ on
     * input/output 1; we form a very simple loop by connecting a
     * ‹delay› with a ‹not› to obtain alternation */

    d.connect( 0, 1, flip );
    flip.connect( 1, 0, d );

    /* delay starts at false → flip starts at true */

    assert(  flip.read( 1 ) );
    assert( !flip.read( 1 ) );
    assert(  flip.read( 1 ) );
    assert( !flip.read( 1 ) );
    assert(  flip.read( 1 ) );

    /* let's latch the output */

    latch l;
    l.connect( 1, 1, flip );

    /* the initial flip output is 0 */
    assert( !l.read( 0 ) );
    assert(  l.read( 0 ) );
    assert(  l.read( 0 ) );
    assert(  l.read( 0 ) );

    /* drive another blinky from the first one, with a delay */

    cnot flip_2;
    delay d_2, d_3;

    d_3.connect( 1, 1, flip );

    d_2.connect( 0, 1, flip_2 );
    flip_2.connect( 0, 0, d );
    flip_2.connect( 1, 1, d_3 );

    assert( !flip_2.read( 1 ) );
    assert(  flip_2.read( 1 ) );
    assert(  flip_2.read( 1 ) );

    /* and true forever */

    return 0;
}
