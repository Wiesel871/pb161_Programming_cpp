#include <cmath>
#include <cassert>

/* Structure ‹angle› simply wraps a single double-precision number,
 * so that we can use constructor overloads to allow use of both
 * polar and cartesian forms to create instances of a single type
 * (‹complex›). */

struct angle;
struct complex;

/* Now implement the following two functions, so that they work both
 * for real and complex numbers. */

// double magnitude( … )
// … reciprocal( … )

/* The following two functions only make sense for complex numbers,
 * where ‹arg› is the argument, normalized into the range ⟦(-π, π⟩⟧:
 * */

double real( complex );
double imag( complex );
double arg( complex );

int main()
{
    double pi = 4 * std::atan( 1 );

    assert( magnitude(  1 ) == 1 );
    assert( magnitude( -1 ) == 1 );

    assert( magnitude( complex( 0,  0 ) ) == 0 );
    assert( magnitude( complex( 1,  0 ) ) == 1 );
    assert( magnitude( complex( 0,  1 ) ) == 1 );
    assert( magnitude( complex( 0, -1 ) ) == 1 );
    assert( magnitude( complex( 3,  4 ) ) == 5 );

    assert( magnitude( complex( 2, angle{ 0    } ) ) == 2 );
    assert( magnitude( complex( 0, angle{ 3.14 } ) ) == 0 );

    assert( reciprocal(  2 ) == 0.5 );
    assert( reciprocal( -2 ) == -0.5 );

    complex c_1 = reciprocal( complex( 1, 0 ) );

    assert( std::fabs( real( c_1 ) - 1 ) < 1e-8 );
    assert( std::fabs( imag( c_1 ) - 0 ) < 1e-8 );

    complex c_2 = reciprocal( complex( 1, 1 ) );

    assert( std::fabs( real( c_2 ) - 0.5 ) < 1e-8 );
    assert( std::fabs( imag( c_2 ) + 0.5 ) < 1e-8 );

    complex one( 1, angle{ 0 } );
    assert( real( reciprocal( one ) ) == 1 );
    assert( imag( reciprocal( one ) ) == 0 );

    complex i( 1, angle{ -pi / 2 } );
    assert( std::fabs( real( reciprocal( i ) ) - 0 ) < 1e-8 );
    assert( std::fabs( imag( reciprocal( i ) ) - 1 ) < 1e-8 );

    return 0;
}
