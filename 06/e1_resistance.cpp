#include <cassert>
#include <cmath>

/* We are given a simple electrical circuit made of resistors and
 * wires, and we want to compute the total resistance between two
 * points. The circuit is simple in the sense that in any given
 * section, all its immediate sub-sections are either connected in
 * series or in parallel. Here is an example:
 *
 *                     ┌────┐
 *              ┌─────╴│ R₂ │╶─────┐
 *      ┌────┐  │      └────┘      │  ┌────┐
 *   ●╶╴│ R₁ │╶╴●                  ●╶╴│ R₅ │╶╴●
 *   A  └────┘  │  ┌────┐  ┌────┐  │  └────┘  B
 *              └─╴│ R₃ │╶╴│ R₄ │╶─┘
 *                 └────┘  └────┘
 *
 * The resistance that we are interested in is between the points A
 * and B. Given ⟦R₁⟧ and ⟦R₂⟧ connected in series, the total
 * resistance is ⟦R = R₁ + R₂⟧. For the same resistors connected in
 * parallel, the resistance is given by ⟦1/R = 1/R₁ + 1/R₂⟧. */

/* You will implement 2 classes: ‹series› and ‹parallel›, each of
 * which represents a single segment of the circuit. Both classes
 * shall provide a method ‹add›, that will accept either a number
 * (‹double›) which will add a single resistor to that segment, or a
 * ‹const› reference to the opposite class (i.e. an instance of
 * ‹series› should accept a reference to ‹parallel› and vice versa).
 */

class series;
class parallel;

/* Then add a top-level function ‹resistance›, which accepts either
 * a ‹series› or a ‹parallel› instance and computes the total
 * resistance of the circuit described by that instance. The exact
 * prototype is up to you. */

int main()
{
    /* the resistor network from the example, with R_n = nΩ */

    series R_34;
    R_34.add( 3 );
    R_34.add( 4 );
    assert( resistance( R_34 ) == 7 );

    parallel par;
    par.add( 2 );
    par.add( R_34 );
    assert( std::fabs( resistance( par ) - 1.5555 ) <= 0.0006 );

    series top;
    top.add( 1 );
    top.add( par );
    top.add( 5 );

    assert( std::fabs( resistance( top ) - 7.5555 ) <= 0.0006 );

    return 0;
}
