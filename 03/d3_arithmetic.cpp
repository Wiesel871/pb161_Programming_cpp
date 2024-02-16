#include <cassert>

/* Operator overloading allows instances of classes to behave more
 * like built-in types: it makes it possible for values of custom
 * types to appear in expressions, as operands. Before we look at
 * examples of how this looks, we need to define a class with some
 * overloaded operators. For binary operators, it is customary to
 * define them using a ‘friends trick’, which allows us to define a
 * top-level function inside a class. */

/* As a very simple example, we will implement a class which
 * represents integral values modulo 7 (this happens to be a finite
 * field, with addition and multiplication). */

struct gf7
{
    int value;

    /* We can name a constant by wrapping it in a method. There are
     * other ways to achieve the same effect, but we don't currently
     * have the necessary pieces of syntax. */

    int modulus() const { return 7; }

    /* A helper method to normalize an integer. We would really
     * prefer to «enforce» the normalization (such that «all» values
     * of type ‹gf7› would have their ‹value› field in the range
     * ⟦⟨0, 7)⟧, but we currently do not have the mechanisms to do
     * that either. This will improve in the next chapter. */

    gf7 normalize() const { return { value % modulus() }; }

    /* This is the ‘friend trick’ syntax for writing operators, and
     * for binary operators, it is often the preferred one (because
     * of its symmetry). The function is not really a part of the
     * compound type in this case – the trick is that we can write
     * it here anyway. The implementation relies on the simple fact
     * that ⟦ [a]₇ + [b]₇ = [a + b]₇ ⟧. */

    friend gf7 operator+( gf7 a, gf7 b )
    {
        return gf7{ a.value + b.value }.normalize();
    }

    /* For multiplication, we will use the more ‘orthodox‘ syntax,
     * where the operator is a ‹const› method: the left operand is
     * passed into the operator as ‹this›, the right operand is the
     * argument. In general, operators-as-methods have one explicit
     * argument less (unary operators take 0 arguments, binary take
     * 1 argument). Note that normally, you would use the same form
     * for all symmetric operators for any given type – we mix them
     * here to highlight the difference. We again use the fact that
     * ⟦ [a]₇⋅[b]₇ = [a⋅b]₇ ⟧. */

    gf7 operator*( gf7 b ) const
    {
        return gf7{ value * b.value }.normalize(); 
    }

    /* Values of type ‹gf7› cannot be directly compared (we did not
     * define the required operators) – instead, we provide this
     * method to convert instances of ‹gf7› back into ‹int›'s. */

    int to_int() const { return value % modulus(); }
};

/* Operators can be also overloaded using ‘normal’ top-level
 * functions, like this unary minus (which finds the additive
 * inverse of the given element). */

gf7 operator-( gf7 x ) { return gf7{ 7 - x.to_int() }; }

/* Now that we have defined the class and the operators, we can look
 * at how is the result used. */

int main() /* demo */
{
    gf7 a{ 3 }, b{ 4 }, c{ 0 }, d{ 5 };

    /* Values ‹a›, ‹b› and so forth can be now directly used in
     * arithmetic expressions, just as we wanted. */

    gf7 x = a + b;
    gf7 y = a * b;

    /* Let us check that the operations work as expected: */

    assert( x.to_int() == c.to_int() ); /* [3]₇ + [4]₇ = [0]₇ */
    assert( y.to_int() == d.to_int() ); /* [3]₇ * [4]₇ = [5]₇ */
    assert( (-a + a).to_int() == c.to_int() ); /* unary minus */
}
