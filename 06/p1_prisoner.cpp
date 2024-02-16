#include <cassert>

/* Another exercise, another class hierarchy. The «abstract base
 * class» will be called ‹prisoner›, and the implementations will be
 * different strategies in the well-known game of (iterated)
 * prisoner's dilemma. */

/* The ‹prisoner› class should provide method ‹betray› which takes a
 * boolean (the decision of the other player in the last round) and
 * returns the decision of the player for this round. In general,
 * the ‹betray› method should «not» be ‹const›, because strategies
 * may want to remember past decisions (though we will not implement
 * a strategy like that in this exercise). */

class prisoner;

/* Implement an always-betray strategy in class ‹traitor›, the
 * tit-for-tat strategy in ‹vengeful› and an always-cooperate in
 * ‹benign›. */

class traitor;
class vengeful;
class benign;

/* Implement a simple strategy evaluator in function ‹play›. It
 * takes two prisoners and the number of rounds and returns a
 * negative number if the first one wins, 0 if the game is a tie and
 * a positive number if the second wins (the absolute value of the
 * return value is the difference in scores achieved). The scoring
 * matrix:
 *
 *  • neither player betrays     2 / 2
 *  • a betrays, b does not:     3 / 0
 *  • a does not betray, b does: 0 / 3
 *  • both betray                1 / 1 */

int play( prisoner &a, prisoner &b, int rounds );

int main()
{
    /* We won't bother with making multiple instances per class,
     * since we know those strategies are stateless. */

    traitor t;
    vengeful v;
    benign b;

    /* Each of the strategies played against itself will result in a
     * tie. */

    assert( play( t, t,  1 ) == 0 );
    assert( play( t, t, 10 ) == 0 );

    assert( play( b, b,  1 ) == 0 );
    assert( play( b, b, 10 ) == 0 );

    assert( play( v, v,  1 ) == 0 );
    assert( play( v, v, 10 ) == 0 );

    /* The benign and vengeful strategies also tie with each other,
     * since vengeful behaves exactly like benign when playing
     * against one. */

    assert( play( v, b, 1 ) == 0 );
    assert( play( b, v, 1 ) == 0 );

    /* The traitor beats both other strategies. */

    assert( play( t, b, 1 ) < 0 );
    assert( play( b, t, 1 ) > 0 );
    assert( play( t, v, 1 ) < 0 );
    assert( play( v, t, 1 ) > 0 );

    assert( play( t, b, 10 ) < 0 );
    assert( play( b, t, 10 ) > 0 );
    assert( play( t, v, 10 ) < 0 );
    assert( play( v, t, 10 ) > 0 );

    return 0;
}
