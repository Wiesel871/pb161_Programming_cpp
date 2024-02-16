#include <cassert>

/* In this example, we will define a class that represents a
 * (physical) force in 3D. Forces are «vectors» (in the mathematical
 * sense): they can be added and multiplied by scalars (scalars are,
 * in this case, real numbers). Forces can also be compared for
 * equality (we will use fuzzy comparison because floating point
 * computations are inexact).
 *
 * Hint: It may be useful to know that when overloading binary
 * operators, the operands do not need to be of the same type. */

class force;

int main()
{
    force zero( 0, 0, 0 ), e_x( 1, 0, 0 ), e_y( 0, 1, 0 ), e_z( 0, 0, 1 );
    assert( 2 * zero == zero );
    assert( e_x + (-1) * e_x == zero );
    assert( e_x + e_y == force( 1, 1, 0 ) );
    assert( e_x + e_x == 2 * e_x );
    assert( e_z + e_x == force( 1, 0, 1 ) );
    assert( 2 * e_x + e_y == force( 2, 1, 0 ) );
    assert( zero + e_x == e_x );
    assert( 0 * e_x == zero );
    assert( 1 * e_x == e_x );

    return 0;
}
