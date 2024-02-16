#include <vector>
#include <cassert>

/* In this demonstration, we will look at overloading: both of
 * regular «methods» and of «constructors». The first class which we
 * will implement is ‹number›, which can represent either a real
 * (floating-point) number or an integer. Besides the attributes
 * ‹integer› and ‹real› which store the respective numbers, the
 * class remembers which type of number it stores, using a boolean
 * attribute called ‹is_real›. */

struct number
{
    bool is_real;
    int integer = 0;
    double real = 0;

    /* We provide two constructors for ‹number›: one for each type
     * of number that we wish to store. The overload is selected
     * based on the type of argument that is provided. */

    number( int i ) : is_real( false ), integer( i ) {}
    number( double r ) : is_real( true ), real( r ) {}
};

/* The second class will be a container of numbers which directly
 * allows the user to insert both floating-point and integer
 * numbers, without converting them to a common type. To make
 * insertion convenient, we provide overloads of the ‹add› method.
 * Access to the numbers is index-based and is provided by the ‹at›
 * method, which is overloaded for entirely different reasons. */

class numbers
{
    /* The sole attribute of the ‹numbers› class is the backing
     * store, which is an ‹std::vector› of ‹number› instances. */

    std::vector< number > _data;
public:

    /* The two ‹add› overloads both construct an appropriate
     * instance of ‹number› and push it to the backing vector.
     * Nothing surprising there. */

    void add( double d ) { _data.emplace_back( d ); }
    void add( int i )    { _data.emplace_back( i ); }

    /* The overloads for ‹at› are much more subtle: notice that the
     * argument types are all identical – there are only 2
     * differences, first is the return type, which however does
     * «not participate» in overload resolution. If two functions
     * only differ in return type, this is an error, since there is
     * no way to select which overload should be used.
     *
     * The other difference is the ‹const› qualifier, which indeed
     * does participate in overload resolution. This is because
     * methods have a hidden argument, ‹this›, and the trailing
     * ‹const› concerns this argument. The ‹const› method is
     * selected when the call is performed on a ‹const› object (most
     * often because the call is done on a constant reference). */

    const number &at( int i ) const { return _data.at( i ); }
    number &at( int i ) { return _data.at( i ); }
};

int main() /* demo */
{
    numbers n;
    n.add( 7 );
    n.add( 3.14 );

    assert( !n.at( 0 ).is_real );
    assert(  n.at( 1 ).is_real );

    assert( n.at( 0 ).integer == 7 );

    /* Notice that it is possible to assign through the ‹at› method,
     * if the object itself is mutable. In this case, overload
     * resolution selects the second overload, which returns a
     * mutable reference to the ‹number› instance stored in the
     * container. */

    n.at( 0 ) = number( 3 );
    assert( n.at( 0 ).integer == 3 );

    /* However, it is still possible to use ‹at› on a constant
     * object – in this case, the resolution picks the first
     * overload, which returns a constant reference to the relevant
     * ‹number› instance. Hence, we cannot change the number this
     * way (as we expect, since the entire object is constant, and
     * hence also each of its components). */

    const numbers &n_const = n;
    assert( n_const.at( 0 ).integer == 3 );

    // n_const.at( 1 ) = number( 1 ); this will not compile
}
