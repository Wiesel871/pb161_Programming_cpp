#include <cmath>
#include <cassert>
#include <tuple>

/* We will go back to a bit of geometry, this time with circles and
 * lines: in this exercise, we will be interested in planar
 * intersections. We will consider two objects to intersect when
 * they have at least one common point. On the C++ side, we will use
 * a bit of a trick with ‹virtual› method overloading (in a slightly
 * more general setting, the trick is known as the «visitor
 * pattern»). */

/* First some definitions: the familiar ‹point›. */

using point = std::pair< double, double >;

/* Check whether two floating-point numbers are ‘essentially the
 * same’ (i.e. fuzzy equality). */

bool close( double a, double b )
{
    return std::fabs( a - b ) < 1e-10;
}

/* We will need to use forward declarations in this exercise, since
 * methods of the base class will refer to the derived types. */

struct circle;
struct line;

/* These two helper functions are already defined in this file and
 * may come in useful (like the ‹slope› class above). */

double dist( point, point );
double dist( const line &, point );

/* A helper class which is constructed from two points. Two
 * instances of ‹slope› compare equal if the slopes of the two lines
 * passing through the respective point pairs are the same. */

struct slope : std::pair< double, double >
{
    slope( point p, point q )
        : point( ( q.first  - p.first  ) / dist( p, q ),
                 ( q.second - p.second ) / dist( p, q ) )
    {}

    bool operator==( const slope &o ) const
    {
        auto [ px, py ] = *this;
        auto [ qx, qy ] = o;

        return ( close( px, qx ) && close( py, qy ) ) ||
               ( close( px, -qx ) && close( py, -qy ) );
    }

    bool operator!=( const slope &o ) const
    {
        return !( *this == o );
    }
};

/* Now we can define the class ‹object›, which will have a ‹virtual›
 * method ‹intersects› with 3 overloads: one that accepts a
 * ‹const› reference to a ‹circle›, another that accepts a
 * ‹const› reference to a ‹line› and finally one that accepts any
 * ‹object›. */

class object;

/* Put definitions of the classes ‹circle› and ‹line› here. A
 * ‹circle› is given by a ‹point› and a radius (‹double›), while a
 * ‹line› is given by two points. NB. Make the ‹line› attributes
 * ‹public› and name them ‹p› and ‹q› to make the ‹dist› helper
 * function work. */

struct circle; /* ref: 18 lines */
struct line;   /* ref: 18 lines */

/* Definitions of the helper functions. */

double dist( point p, point q )
{
    auto [ px, py ] = p;
    auto [ qx, qy ] = q;
    return std::sqrt( std::pow( px - qx, 2 ) +
                      std::pow( py - qy, 2 ) );
}

double dist( const line &l, point p )
{
    auto [ x2, y2 ] = l.q;
    auto [ x1, y1 ] = l.p;
    auto [ x0, y0 ] = p;

    return std::fabs( ( y2 - y1 ) * x0 - ( x2 - x1 ) * y0 +
                      x2 * y1 - y2 * x1 ) /
           dist( l.p, l.q );
}

int main()
{
    circle c_unit( { 0, 0 }, 1 ),
           c_shifted( { 1, 0 }, 1 ),
           c_big( { 0, 0 }, 2 ),
           c_distant( { 3, 0 }, 1 ),
           c_touchy( { 3, 0 }, 2 );

    line l1( { 0, 0 }, { 1, 0 } ),     // horizontal
         l2( { 1, 0 }, { 1, 1 } ),     // vertical at x = 1
         l3( { 0.5, 0 }, { 0.5, 1 } ), // vertical at x = 0.5
         l4( { 4, 0 }, { 4, 1 } ),     // vertical at x = 4
         l5( { 0, 0 }, { 1, 1 } ),     // at 45° angle through [0, 0]
         l6( { 1, 0 }, { 2, 1 } );     // at 45° angle through [1, 0]

    /* circles vs circles */

    assert(  c_unit.intersects( c_unit ) );

    assert(  c_unit.intersects( c_shifted ) );
    assert(  c_shifted.intersects( c_unit ) );
    assert( !c_unit.intersects( c_big ) );
    assert( !c_big.intersects( c_unit ) );
    assert(  c_shifted.intersects( c_big ) );
    assert(  c_big.intersects( c_shifted ) );

    assert( !c_unit.intersects( c_distant ) );
    assert( !c_distant.intersects( c_unit ) );
    assert(  c_unit.intersects( c_touchy ) );
    assert(  c_touchy.intersects( c_unit ) );
    assert( !c_distant.intersects( c_touchy ) );
    assert( !c_touchy.intersects( c_distant ) );

    /* lines vs lines */

    assert(  l1.intersects( l1 ) );

    /* vertical intersects horizontal */
    assert(  l1.intersects( l2 ) &&  l2.intersects( l1 ) );

    /* distinct verticals do not intersect */
    assert( !l2.intersects( l3 ) && !l3.intersects( l2 ) );
    assert( !l2.intersects( l4 ) && !l4.intersects( l2 ) );

    /* the diagonal line intersects all the vertical/horizontal */
    assert( l5.intersects( l1 ) );
    assert( l5.intersects( l2 ) );
    assert( l5.intersects( l3 ) );
    assert( l5.intersects( l4 ) );

    assert( !l6.intersects( l5 ) && !l5.intersects( l6 ) );

    /* circles vs lines */

    assert( l1.intersects( c_unit ) );
    assert(  c_unit.intersects( l1 ) );
    assert(  c_unit.intersects( l2 ) );
    assert(  c_unit.intersects( l3 ) );
    assert( !c_unit.intersects( l4 ) );
    assert(  c_unit.intersects( l5 ) );
    assert(  c_unit.intersects( l6 ) );

    assert(  c_shifted.intersects( l1 ) );
    assert(  c_shifted.intersects( l2 ) );
    assert(  c_shifted.intersects( l3 ) );
    assert( !c_shifted.intersects( l4 ) );
    assert(  c_shifted.intersects( l5 ) );
    assert(  c_shifted.intersects( l6 ) );

    assert(  c_big.intersects( l1 ) );
    assert(  c_big.intersects( l2 ) );
    assert(  c_big.intersects( l3 ) );
    assert( !c_big.intersects( l4 ) );
    assert(  c_big.intersects( l5 ) );
    assert(  c_big.intersects( l6 ) );

    assert(  c_distant.intersects( l1 ) );
    assert( !c_distant.intersects( l2 ) );
    assert( !c_distant.intersects( l3 ) );
    assert(  c_distant.intersects( l4 ) );
    assert( !c_distant.intersects( l5 ) );
    assert( !c_distant.intersects( l6 ) );

    assert(  c_touchy.intersects( l1 ) );
    assert(  c_touchy.intersects( l2 ) );
    assert( !c_touchy.intersects( l3 ) );
    assert(  c_touchy.intersects( l4 ) );
    assert( !c_touchy.intersects( l5 ) );
    assert(  c_touchy.intersects( l6 ) );

    const object &a = c_shifted, &b = l3, &c = l4;
    assert(  a.intersects( a ) );
    assert(  b.intersects( b ) );
    assert(  a.intersects( b ) );
    assert(  b.intersects( a ) );
    assert( !a.intersects( c ) );
    assert( !b.intersects( c ) );
    assert( !c.intersects( a ) );
    assert( !c.intersects( b ) );

    return 0;
}
