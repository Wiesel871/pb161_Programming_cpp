#include <cassert>

/* There should be 4 classes: the base class ‹gesture› and 3
 * derived: ‹rock›, ‹paper› and ‹scissors›. Class ‹gesture› has a
 * (pure virtual) method ‹fight› which takes another gesture (via a
 * const reference) and returns ‹true› if the current gesture wins.
 *
 * To do this, add another method, ‹visit›, which has 3 overloads,
 * one each for ‹rock›, ‹paper› and ‹scissors›. Then override
 * ‹fight› in each derived class, to simply call ‹visit( *this )› on
 * the opposing gesture. The ‹visit› method knows the type of both
 * ‹this› and the opponent (via the overload) – simply indicate the
 * winner by returning an appropriate constant. */

class rock;
class paper;
class scissors;

/* Keep the forward declarations, you will need them to define
 * the overloads for ‹visit›. */

class gesture;

/* Now define the 3 derived classes. */

int main()
{
    rock r_;
    paper p_;
    scissors s_;

    const gesture &r = r_, &p = p_, &s = s_;

    assert( r.fight( s ) );
    assert( s.fight( p ) );
    assert( p.fight( r ) );

    assert( !r.fight( r ) );
    assert( !s.fight( s ) );
    assert( !p.fight( p ) );

    assert( !r.fight( p ) );
    assert( !s.fight( r ) );
    assert( !p.fight( s ) );

    return 0;
}
