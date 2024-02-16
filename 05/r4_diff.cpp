#include <cassert>
#include <cmath>

/* In this exercise, we will implement automatic differentiation of
 * simple expressions. You will need the following rules:
 *
 *  • linearity: ⟦ (a⋅f(x) + b⋅g(x))' = a⋅f'(x) + b⋅g'(x) ⟧
 *  • the Leibniz rule: ⟦ (f(x)⋅g(x))' = f'(x)⋅g(x) + f(x)⋅g'(x) ⟧
 *  • chain rule: ⟦ (f(g(x)))' = f'(g(x))⋅g'(x) ⟧
 *  • derivative of exponential: ⟦ exp'(x) = exp(x) ⟧ */

/* Define a type, ‹expr› (from «expression»), such that values of
 * this type can be constructed from integers, added and multiplied,
 * and exponentiated using function ‹expnat› (to avoid conflicts
 * with the ‹exp› in the standard library). */

class expr; /* ref: 29 + 7 lines */
expr expnat( expr );

/* Implement function ‹diff› that accepts a single ‹expr› and
 * returns the derivative (again in the form of ‹expr›). Define a
 * constant ‹x› of type ‹expr› such that ‹diff( x )› is 1. */

expr diff( expr ); /* ref: 11 lines */
// const expr x;

/* Finally, implement function ‹eval› which takes an ‹expr› and a
 * ‹double› and it substitutes for ‹x› and computes the value of the
 * expression. */

double eval( expr, double ); /* ref: 11 lines */

int main()
{
    auto chk = []( expr e, double x_val, double o )
    {
        return std::fabs( eval( e, x_val ) - o ) < 1e-10;
    };

    auto chk_diff = [&]( expr e, double x_val, double o )
    {
        return chk( diff( e ), x_val, o );
    };

    assert( chk_diff( x, 0, 1 ) );
    assert( chk_diff( 2 * x, 0, 2 ) );
    assert( chk_diff( x * x, 0, 0 ) );
    assert( chk_diff( x * x, 1, 2 ) );
    assert( chk( expnat( x ), 0, 1 ) );
    assert( chk( x * expnat( x ), 0, 0 ) );
    assert( chk_diff( expnat( x ), 0, 1 ) );
    assert( chk_diff( expnat( x * x ), 0, 0 ) );
    assert( chk_diff( expnat( x * x ), 1, 2 * exp( 1 ) ) );
    assert( chk_diff( expnat( x * x ), 2, 4 * exp( 4 ) ) );
    assert( chk_diff( x * expnat( x ), 1, 2 * exp( 1 ) ) );
    assert( chk_diff( x * expnat( x ), 2, 3 * exp( 2 ) ) );
    assert( chk_diff( 2 * expnat( x ), 1, 2 * exp( 1 ) ) );
    assert( chk_diff( x + expnat( x ), 2, 1 + exp( 2 ) ) );

    return 0;
}
