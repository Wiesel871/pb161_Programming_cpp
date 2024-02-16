#include <cassert>
#include <cstdint>
#include <compare>

/* In this example, we will show relational operators, which are
 * very similar to the arithmetic operators from previous example,
 * except for their return types, which are ‹bool› values. */

/* The example which we will use in this case are sets of small
 * natural numbers (1-64) with inclusion as the order. We will
 * use three-way comparison to obtain most of the comparison
 * operators ‘for free’.
 *
 * NB. Standard ordered containers like ‹std::set› and ‹std::map›
 * require the operator less-than to define a «strict weak ordering»
 * (which is corresponds to a «total» order). The comparison
 * operators in this example do «not» define a total order (some
 * sets are «incomparable»). */

struct set
{
    /* Each bit of the below number indicates the presence of the
     * corresponding integer (the index of that bit) in the set. */

    uint64_t bits = 0;

    /* We also define a few methods to add and remove numbers from
     * the set, to test for presence of a number and an emptiness
     * check. */

    void add( int i ) { bits |=    1ul << i; }
    void del( int i ) { bits &= ~( 1ul << i ); }
    bool has( int i ) const { return bits & ( 1ul << i ); }
    bool empty() const { return !bits; }

    /* We will use the method syntax here, because it is slightly
     * shorter. We start with (in)equality, which is very simple
     * (the sets are equal when they have the same members). Note
     * that defining a separate operator ‹!=› is not required in
     * C++20. */

    bool operator==( set b ) const { return bits == b.bits; }

    /* It will be quite useful to have set difference to implement
     * the comparisons below, so let us also define that: */

    set operator-( set b ) const { return { bits & ~b.bits }; }

    /* Since the non-strict comparison (ordering) operators are
     * easier to implement, we will do that first. Set ‹b› is a
     * superset of set ‹a› if all elements of ‹a› are also present
     * in ‹b›, which is the same as the difference ‹a - b› being
     * empty. We will write a single comparison operator, then use
     * it to implement three-way comparison, which the compiler will
     * then use to derive all the remaining comparison operators. */

    bool operator<=( set b ) const { return ( *this - b ).empty(); }

    /* In addition to getting all other comparisons for free, the
     * three-way comparison also allows us to declare the properties
     * of the ordering. */

    friend std::partial_ordering operator<=>( set a, set b )
    {
        if ( a == b )
            return std::partial_ordering::equivalent;
        if ( a <= b )
            return std::partial_ordering::less;
        if ( b <= a )
            return std::partial_ordering::greater;

        return std::partial_ordering::unordered;
    }
};

int main() /* demo */
{
    set a; a.add( 1 ); a.add( 7 ); a.add( 13 );
    set b; b.add( 1 ); b.add( 6 ); b.add( 13 );

    /* In each pair of assertions below, the two expressions are not
     * quite equivalent. Do you understand why? */

    assert( a != b ); assert( !( a == b ) );
    assert( a == a ); assert( !( a != a ) );

    /* The two sets are incomparable, i.e. neither is less than the
     * other, but as shown above they are not equal either. */
    assert( !( a < b ) ); assert( !( b < a ) );

    a.add( 6 ); // let's make ‹a› a superset of ‹b›

    /* And check that the ordering operators work on ordered sets. */

    assert( a > b ); assert( a >= b ); assert( a != b );
    assert( b < a ); assert( b <= a ); assert( b != a );

    b.add( 7 ); /* let's make the sets equal */
    assert( a == b ); assert( a <= b ); assert( a >= b );
}
