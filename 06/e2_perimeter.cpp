#include <cmath>
#include <cassert>

/* Implement a simple inheritance hierarchy – the base class will be
 * ‹shape›, with a pure virtual method ‹perimeter›, the 2 derived
 * classes will be ‹circle› and ‹rectangle›. The circle is
 * constructed from a radius, while the rectangle from a width and
 * height, all of them floating-point numbers. */

class shape;
class circle;
class rectangle;

bool check_shape( const shape &s, double p )
{
    return std::fabs( s.perimeter() - p ) < 1e-8;
}

int main()
{
    double pi = 4 * std::atan( 1 );

    circle c_1( 1 ), c_2( 2 );
    rectangle r_1( 1, 1 ), r_2( 3, 4 );

    assert( check_shape( c_1, 2 * pi ) );
    assert( check_shape( c_2, 4 * pi ) );
    assert( check_shape( r_1, 4 ) );
    assert( check_shape( r_2, 14 ) );

    return 0;
}
