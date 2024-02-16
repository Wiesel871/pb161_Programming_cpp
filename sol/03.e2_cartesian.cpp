/* This is a solution that uses the friend syntax. For a solution
 * which uses the method syntax, see ‹cartesian.alt.cpp›. */

struct cartesian
{
    double real, imag;

    friend cartesian operator+( cartesian a, cartesian b )
    {
        /* You may not know this syntax yet. In a return statement,
         * braces without a type name call the constructor of the
         * return type. I.e. ‹{ a, b }› in this context is the same
         * as ‹cartesian( a, b )›. */

        return { a.real + b.real, a.imag + b.imag };
    }

    friend cartesian operator-( cartesian a, cartesian b )
    {
        return { a.real - b.real, a.imag - b.imag };
    }

    friend cartesian operator-( cartesian a )
    {
        return { -a.real, -a.imag };
    }

    friend bool operator==( cartesian a, cartesian b )
    {
        return a.real == b.real && a.imag == b.imag;
    }
};

cartesian make_cartesian( double r, double i )
{
    return { .real = r, .imag = i };
}
