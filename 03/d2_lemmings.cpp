#include <cassert>
#include <vector>

/* While we are talking about computer games, you might have heard
 * about a game called Lemmings (but it's not super important if you
 * didn't).  In each level of the game, lemmings start spawning at a
 * designated location, and immediately start to wander about, fall
 * off cliffs, drown and generally get hurt. The player is in charge
 * of saving them (or rather as many as possible), by giving them
 * tasks like digging tunnels, or stopping and redirecting other
 * lemmings.
 *
 * Let's try to design a type which will capture the state of a
 * single lemming: */

struct lemming
{
    /* Each lemming is located somewhere on the map: coordinates
     * would be a good way to describe this. For simplicity, let's
     * say the designated spawning spot is at coordinates ⟦(0, 0)⟧. */

    double _x = 0, _y = 0;

    /* Unless they hit an obstacle, lemmings simply walk in a given
     * direction – this is another candidate for an attribute; and
     * being rather heedless, it's probably good idea to keep track
     * of whether they are still alive. */

    bool _facing_right = true;
    bool _alive = true;

    /* Finally, they might be assigned a task, which they will
     * immediately start performing. The exact meaning of the number
     * is not very important. */

    int _task = 0;

    /* Let us define a few (mostly self-explanatory) methods: */

    void start_digging() { _task = 1; }

    bool busy()  const { return _task != 0; }
    bool alive() const { return _alive; }

    void step()
    {
        _x += _facing_right ? 1 : -1;
        _y += 0; // TODO gravity, terrain, …
    }
};

/* Earlier, we have mentioned that user-defined types are
 * essentially the same as built-in types – their values can be
 * stored in variables, passed to and from functions and so on.
 * There are more ways in which this is true: for instance, we can
 * construct collections of such values. Earlier, we have seen a
 * sequence of integers, the type of which was ‹std::vector< int >›.
 * We can create a vector of lemmings just as easily: as an
 * ‹std::vector< lemming >›. Let us try: */

int count_busy( const std::vector< lemming > &lemmings )
{

    /* Note that the vector is marked ‹const› (because it is passed
     * into the function as a «constant reference»). That extends to
     * the items of the vector: the individual lemmings are also
     * ‹const›. We are not allowed to call non-‹const› methods, or
     * assign into their attributes here. For instance, calling
     * ‹lemmings[ 0 ].start_digging()› would be a compile error. */

    int count = 0;

    /* Of course we can iterate a vector of lemmings like any other
     * vector, and call methods on the individual lemmings («inside»
     * the vector, since we are using a reference). */

    for ( const lemming &l : lemmings )
        if ( l.busy() )
            count ++;

    return count;
}

int main() /* demo */
{
    /* We first create an (empty) vector, then fill it in with 7
     * lemmings. */

    std::vector< lemming > lemmings;
    lemmings.resize( 7 );

    /* We can call methods on the lemmings as usual, by indexing the
     * vector: */

    lemmings[ 0 ].start_digging();
    assert( count_busy( lemmings ) == 1 );

    /* We can also modify the lemmings in a range ‹for› loop –
     * notice the absence of ‹const›; this time, we use a «mutable
     * reference» – the lemmings are modified «in place» inside the
     * container. */

    for ( lemming &l : lemmings )
    {
        assert( l.alive() );
        l.start_digging();
    }

    assert( count_busy( lemmings ) == 7 );
}
