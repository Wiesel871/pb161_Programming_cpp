#include <cassert>
#include <cmath>

/* Standard 2D point. */

struct point;

/* Implement a structure ‹circle› with 2 constructors, one of which
 * accepts a point and a number (center and radius) and another
 * which accepts 2 points (center and a point on the circle itself).
 * Store the circle using its center and radius, in attributes
 * ‹center› and ‹radius› respectively. */

struct circle;

int main()
{
    point p_1( 0, 0 ), p_2( 1, 0 ), p_3( 3, 4 );
    circle c_1( p_1, 1 ), c_2( p_2, 2 ),
           c_3( p_1, p_2 ), c_4( p_1, p_3 );

    assert( std::fabs( c_1.radius - 1 ) < 1e-8 );
    assert( std::fabs( c_2.radius - 2 ) < 1e-8 );
    assert( std::fabs( c_3.radius - 1 ) < 1e-8 );
    assert( std::fabs( c_4.radius - 5 ) < 1e-8 );

    return 0;
}
