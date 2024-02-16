#include <cassert>
#include <cmath>
#include <sstream>

/* We have seen the basics of input and output, and that formatted
 * input and output is realized using operators. Like many other
 * operators in C++, those operators can be overloaded. We will show
 * how that works in this example. */

/* We will revisit the ‹cartesian› class from last week, to
 * represent complex numbers in algebraic form, i.e. as a sum of a
 * real and an imaginary number. We do not care about arithmetic
 * this time: we will only implement a constructor and the formatted
 * input and output operators. We will, however, need equality so
 * that we can write test cases. */

class cartesian
{
    double real, imag;
public:

    /* We have seen default arguments before: those are used when no
     * value is supplied by the caller. This also allows instances
     * to be default-constructed. */

    cartesian( double r = 0, double i = 0 ) : real( r ), imag( i ) {}

    /* The comparison is fuzzy, due to the limited precision
     * available in ‹double›. */

    friend bool operator==( cartesian a, cartesian b )
    {
        return std::fabs( a.real - b.real ) < 1e-10 &&
               std::fabs( a.imag - b.imag ) < 1e-10;
    }

    /* Now the formatted output, which is a little easier than the
     * input. Since the first operand of this operator is «not» an
     * instance of ‹cartesian›, the operator «cannot» be implemented
     * as a method. It must either be a function outside the class,
     * or use the ‘friend trick’. Since we will need to access
     * private attributes in the operator, we will use the ‹friend›
     * syntax here. The return type and the type of the first
     * argument are pretty much given and are always the same.
     * You could consider them part of the syntax. The second
     * argument is an instance of our class (this would often be
     * passed as a ‹const› reference). */

    friend std::ostream &operator<<( std::ostream &o, cartesian c )
    {
        /* We will use ‹27.3±7.1*i› as the output format. We can use
         * ‘simpler’ overloads of the ‹<<› operator to build up
         * ours: this is a fairly common practice. We write to the
         * ‹ostream› instance given to us in the argument. We must
         * not forget to return that instance to our caller. */

        o << c.real;
        if ( c.imag >= 0 )
            o << "+";
        return o << c.imag << "*i";
    }

    /* The input operator is similar. It gets a reference to an
     * ‹std::istream› as an argument (and has to pass it along in
     * the return value). The main difference is that the object
     * into which we read the data must be passed as a non-constant
     * (i.e. mutable) reference, since we need to change it. */

    friend std::istream &operator>>( std::istream &i, cartesian &c )
    {
        /* Like above, we will build up our implementation from
         * simpler overloads of the same operator (which all come
         * from the standard library). The formatted input operators
         * for numbers do not require that the number is followed by
         * whitespace, but will stop at a character which can no
         * longer be part of the number. A ‹+› or ‹-› character in
         * the middle of the number qualifies. */

        i >> c.real;

        /* We will slightly abuse the flexibility of the formatted
         * input operator for ‹double› values: it accepts numbers
         * starting with an explicit ‹+› sign, hence we do not need
         * to check the sign ourselves. Just read the imaginary
         * part. */

        i >> c.imag;

        /* We do need to deal with the trailing ‹*i› though. */

        char ch;

        /* When formatted input fails, it should set a ‹failbit› in
         * the input stream. This is how the ‹if ( stream >> value
         * )› construct works. */

        if ( !( i >> ch ) || ch != '*' ||
             !( i >> ch ) || ch != 'i' )
            i.setstate( i.failbit );

        /* And as mentioned above, we need to return a reference to
         * the input stream. */

        return i;
    }

};

int main() /* demo */
{
    std::ostringstream ostr;
    ostr << cartesian( 1, 1 );

    /* We first check that the output behaves as we expected. */

    assert( ostr.str() == "1+1*i" );

    /* We write a few more complex numbers into the stream, using
     * operator chaining. */

    ostr << " " << cartesian( 3, 0 ) << " " << cartesian( 1, -1 )
         << " " << cartesian( 0, 0 );

    assert( ostr.str() == "1+1*i 3+0*i 1-1*i 0+0*i" );

    /* We now construct an input stream from the string which we
     * created above, and check that the values can be read back. */

    std::istringstream istr( ostr.str() );
    cartesian a, b, c;

    /* Let's read back the first number and check that the result
     * makes sense. */

    assert( istr >> a );
    assert( a == cartesian( 1, 1 ) );

    /* We can also check that chaining works as expected, using the
     * remaining numbers in the string. */

    assert( istr >> a >> b >> c );

    assert( a == cartesian( 3, 0 ) );
    assert( b == cartesian( 1, -1 ) );
    assert( c == cartesian( 0, 0 ) );

    /* We can reset an ‹istringstream› by calling its ‹str› method
     * with a new buffer. We want to demonstrate that trying to read
     * an ill-formatted complex number will fail. */

    std::istringstream bad1( "7+3*j" );
    assert( !( bad1 >> a ) );

    std::istringstream bad2( "7" );
    assert( !( bad2 >> a ) );
}
