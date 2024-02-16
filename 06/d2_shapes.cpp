#include <cassert>
#include <cmath>
#include <utility>

/* The inheritance model in C++ is an instance of a more general
 * notion, known as «subtyping». The defining characteristic of
 * subtyping is the Liskov substitution principle: a value which
 * belongs to a «subtype» (a derived class) can be used whenever a
 * variable stores, or a formal argument expects, a value that
 * belongs to a «supertype» (the base class). As mentioned earlier,
 * in C++ this only extends to values passed by «reference» or
 * through pointers. */

/* We will first define a couple useful type aliases to represent
 * points and bounding boxes. */

using point = std::pair< double, double >;
using bounding_box = std::pair< point, point >;

/* Subtype polymorphism is, in C++, implemented via «late binding»:
 * the decision which method should be called is postponed to
 * runtime (with normal functions and methods, this happens during
 * compile time). The decision whether to use early binding (static
 * dispatch) or late binding (dynamic dispatch) is made by the
 * programmer on a method-by-method basis. In other words, some
 * methods of a class can use static dispatch, while others use
 * dynamic dispatch. */

class shape
{
public:

    /* To instruct the compiler to use dynamic dispatch for a given
     * method, put the keyword ‹virtual› in front of that method's
     * return type. Unlike normal methods, a ‹virtual› method may be
     * left «unimplemented»: this is denoted by the ‹= 0› at the end
     * of the declaration. If a class has a method like this, it is
     * marked as «abstract» and it becomes impossible to create
     * instances of this class: the only way to use it is as a «base
     * class», through inheritance. This is commonly done to define
     * «interfaces». In our case, we will declare two such methods.
     */

    virtual double area() const = 0;
    virtual bounding_box box() const = 0;

    /* A class which introduces ‹virtual› methods also needs to have
     * a «destructor» marked as ‹virtual›. We will discuss this in
     * more detail in a later unit. For now, simply consider this to
     * be an arbitrary rule. */

    virtual ~shape() = default;
};

/* As soon as the interface is defined, we can start working with
 * arbitrary classes which implement this interface, even those that
 * have not been defined yet. We will start by writing a simple
 * «polymorphic function» which accepts arbitrary shapes and
 * computes the ratio of their area to the area of their bounding
 * box. */

double box_coverage( const shape &s )
{
    /* Hopefully, you remember structured bindings (if not, revisit
     * e.g. ‹03/rel.cpp›). */

    auto [ ll, ur ] = s.box();
    auto [ left, bottom ] = ll;
    auto [ right, top ] = ur;

    return s.area() / ( ( right - left ) * ( top - bottom ) );
}

/* Another function: this time, it accepts two instances of ‹shape›.
 * The values it actually receives may be, however, of any type
 * derived from ‹shape›. In fact, ‹a› and ‹b› may be each an
 * instances of a different derived class. */

bool box_collide( const shape &sh_a, const shape &sh_b )
{
    /* A helper function (lambda) to decide whether a point is
     * inside (or on the boundary) of a bounding box. */

    auto in_box = []( const bounding_box &box, const point &pt )
    {
        auto [ x, y ] = pt;
        auto [ ll, ur ] = box;
        auto [ left, bottom ] = ll;
        auto [ right, top ] = ur;

        return x >= left && x <= right && y >= bottom && y <= top;
    };

    auto [ a, b ] = sh_a.box();
    auto box = sh_b.box();

    /* The two boxes collide if either of the corners of one is in
     * the other box. */

    return in_box( box, a ) || in_box( box, b );
}

/* We now have the interface and two functions that are defined in
 * terms of that interface. To make some use of the functions,
 * however, we need to be able to make instances of ‹shape›, and as
 * we have seen earlier, that is only possible by deriving classes
 * which provide implementations of the virtual methods declared in
 * the base class. Let's start by defining a circle. */

class circle : public shape
{
    point _center;
    double _radius;
public:

    /* The base class has a default constructor, so we do not need
     * to explicitly call it here. */

    circle( point c, double r ) : _center( c ), _radius( r ) {}

    /* Now we need to implement the ‹virtual› methods defined in the
     * base class. In this case, we can omit the ‹virtual› keyword,
     * but we should specify that this method «overrides» one from a
     * base class. This informs the compiler of our «intention» to
     * provide an implementation to an inherited method and allows
     * it (the compiler) to emit a warning in case we accidentally
     * «hide» the method instead, by mistyping the signature. The
     * most common mistake is forgetting the trailing ‹const›.
     * Please always specify ‹override› where it is applicable. */

    double area() const override
    {
        return 4 * std::atan( 1 ) * std::pow( _radius, 2 );
    }

    /* Now the other ‹virtual› method. */

    bounding_box box() const override
    {
        auto [ x, y ] = _center;
        double r = _radius;
        return { { x - r, y - r }, { x + r, y + r } };
    }
};

/* And a second shape type, so we can actually make some use of
 * polymorphism. Everything is the same as above. */

class rectangle : public shape
{
    point _ll, _ur; /* lower left, upper right */
public:

    rectangle( point ll, point ur ) : _ll( ll ), _ur( ur ) {}

    double area() const override
    {
        auto [ left, bottom ] = _ll;
        auto [ right, top ] = _ur;
        return ( right - left ) * ( top - bottom );
    }

    bounding_box box() const override
    {
        return { _ll, _ur };
    }
};

int main() /* demo */
{
    /* We cannot directly construct a ‹shape›, since it is
     * «abstract», i.e. it has unimplemented «pure virtual methods».
     * However, both ‹circle› and ‹rectangle› provide
     * implementations of those methods which we can use. */

    rectangle square( { 0, 0 }, { 1, 1 } );
    assert( square.area() == 1 );
    assert( square.box() == bounding_box( { 0, 0 }, { 1, 1 } ) );
    assert( box_coverage( square ) == 1 );

    circle circ( { 0, 0 }, 1 );

    /* Check that the area of a unit circle is π, and the ratio of
     * its area to its bounding box is π / 4. */

    double pi = 4 * std::atan( 1 );
    assert( std::fabs( circ.area() - pi ) < 1e-10 );
    assert( std::fabs( box_coverage( circ ) - pi / 4 ) < 1e-10 );

    /* The two shapes quite clearly collide, and if they collide,
     * their bounding boxes must also collide. A shape should always
     * collide with itself, and collisions are symmetric, so let's
     * check that too. */

    assert( box_collide( square, circ ) );
    assert( box_collide( circ, square ) );
    assert( box_collide( square, square ) );
    assert( box_collide( circ, circ ) );

    /* Let's make a shape a bit further out and check the collision
     * detection with that. */

    circle c1( { 2, 3 }, 1 ), c2( { -1, -1 }, 1 );
    assert( !box_collide( circ, c1 ) );
    assert( !box_collide( c1, c2 ) );
    assert( !box_collide( c1, square ) );
    assert(  box_collide( c2, square ) );
}
