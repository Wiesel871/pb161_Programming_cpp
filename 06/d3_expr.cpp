#include <cassert>
#include <stdexcept>

/* To better understand polymorphism, we will need to set up some
 * terminology, particularly:
 *
 *  • the notion of a «static type», which is, essentially, the type
 *    written down in the source code, and of a
 *  • «dynamic type» (also known as a «runtime type»), which is the
 *    actual type of the value that is stored behind a given
 *    reference (or pointer).
 *
 * The relationship between the «static» and «dynamic» type may be:
 *
 *  • the static and dynamic type are the same (this was always the
 *    case until this week), or
 *  • the dynamic type may be a «subtype» of the static type (we
 *    will see that in a short while).
 *
 * Anything else is a bug. */

/* We will use a very simple representation of arithmetic
 * expressions as our example here. An expression is a «tree», where
 * each «node» carries either a «value» or an «operation». We will
 * want to explicitly track the type of each node, and for that, we
 * will use an «enumerated type». Those work the same as in C, but
 * if we declare them using ‹enum class›, the enumerated names will
 * be «scoped»: we use them as ‹type::sum›, instead of just ‹sum› as
 * would be the case in C. */

enum class type { sum, product, constant };

/* Now for the class hierarchy. The base class will be ‹node›. */

class node
{
public:

    /* The first thing we will implement is a ‹static_type› method,
     * which tells us the static type of this class. The base class,
     * however, does not have any sensible value to return here, so
     * we will just throw an exception. */

    type static_type() const
    {
        throw std::logic_error( "bad static_type() call" );
    }

    /* The ‘real’ (dynamic) type must be a ‹virtual› method, since
     * the actual implementation must be selected based on the
     * «dynamic type»: this is exactly what late binding does. Since
     * the method is ‹virtual›, we do not need to supply an
     * implementation if we can't give a sensible one. */

    virtual type dynamic_type() const = 0;

    /* The interesting thing that is associated with each node is
     * its «value». For operation nodes, it can be computed, while
     * for leaf nodes (type ‹constant›), it is simply stored in the
     * node. */

    virtual int value() const = 0;

    /* We also observe the «virtual destructor rule». */

    virtual ~node() = default;
};

/* We first define the (simpler) leaf nodes, i.e. constants. */

class constant : public node
{
    int _value;
public:

    /* The leaf node constructor simply takes an integer value and
     * stores it in an attribute. */

    constant( int v ) : _value( v ) {}

    /* Now the interface common to all ‹node› instances: */

    type static_type() const { return type::constant; }

    /* In methods of class ‹constant›, the «static type» of ‹this›
     * is always¹ either ‹constant *› or ‹const constant *›. Hence
     * we can simply call the ‹static_type› method, since it uses
     * «static dispatch» (it was not declared ‹virtual› in the base
     * class) and hence the call will always resolve to the method
     * just above. */

    type dynamic_type() const override { return static_type(); }

    /* Finally, the ‘business’ method: */

    int value() const override { return _value; }
};

/* ¹ As long as we pretend that the ‹volatile› keyword does not
 *   exist, which is an entirely reasonable thing to do. */

/* The inner nodes of the tree are «operations». We will create an
 * intermediate (but still abstract) class, to serve as a base for
 * the two operation classes which we will define later. */

class operation : public node
{
    const node &_left, &_right;

public:
    operation( const node &l, const node &r )
        : _left( l ), _right( r )
    {}

    /* We will leave ‹static_type› untouched: the version from the
     * base class works okay for us, since there is nothing better
     * that we could do here. The ‹dynamic_type› and ‹value› stay
     * unimplemented. */

    /* We are facing a dilemma here, though. We would like to add
     * accessors for the children, but it is not clear whether to
     * make them ‹virtual› or not. Considering that we keep the
     * references in attributes of this class, it seems unlikely
     * that the implementation of the accessors would change in a
     * subclass and we can use cheaper «static dispatch». */

    const node &left() const { return _left; }
    const node &right() const { return _right; }
};

/* Now for the two operation classes. */

class sum : public operation
{
public:

    /* The base class does not have a default constructor, which
     * means we need to call the one that's available manually. */

    sum( const node &l, const node &r )
        : operation( l, r )
    {}

    /* We want to replace the ‹static_type› implementation that was
     * inherited from ‹node› (through ‹operation›): */

    type static_type() const { return type::sum; }

    /* And now the (dynamic-dispatch) interface mandated by the
     * (indirect) base class ‹node›. We can use the same approach
     * that we used in ‹constant› for ‹dynamic_type›: */

    type dynamic_type() const override { return static_type(); }

    /* And finally the logic. The «static return type» of ‹left› and
     * ‹right› is ‹const node &›, but the method we call on each,
     * ‹value›, uses dynamic dispatch (it is marked ‹virtual› in
     * class ‹node›). Therefore, the actual method which will be
     * called depends on the «dynamic type» of the respective child
     * node. */

    int value() const override
    {
        return left().value() + right().value();
    }
};

/* Basically a re-run of ‹sum›. */

class product : public operation
{
public:

    /* We will use a trick which will allow us to not type out the
     * (boring and redundant) constructor. If all we want to do is
     * just forward arguments to the parent class, we can use the
     * following syntax. You do not have to remember it, but it can
     * save some typing if you do. */

    using operation::operation;

    /* Now the interface methods. */

    type static_type() const { return type::product; }
    type dynamic_type() const override { return static_type(); }

    int value() const override
    {
        return left().value() * right().value();
    }
};

int main() /* demo */
{
    /* Instances of class ‹constant› are quite straightforward.
     * Let's declare some. */

    constant const_1( 1 ),
             const_2( 2 ),
             const_m1( -1 ),
             const_10( 10 );

    /* The constructor of ‹sum› accepts two instances of ‹node›,
     * passed by reference. Since ‹constant› is a subclass of
     * ‹node›, it is okay to use those, too. */

    sum sum_0( const_1, const_m1 ),
        sum_3( const_1, const_2 );

    /* The ‹product› constructor is the same. But now we will also
     * try using instances of ‹sum›, since ‹sum› is also derived
     * (even if indirectly) from ‹node› and therefore ‹sum› is a
     * subtype of ‹node›, too. */

    product prod_4( const_2, const_2 ),
            prod_6( const_2, sum_3 ),
            prod_40( prod_4, const_10 );

    /* Let's also make a ‹sum› instance which has children of
     * different types. */

    sum sum_9( sum_3, prod_6 );

    /* For all variables which hold «values» (i.e. not references),
     * «static type» = «dynamic type». To make the following code
     * easier to follow, the static type of each of the above
     * variables is explicitly mentioned in its name. */

    /* Clearly, we can call the ‹value› method on the variables
     * directly and it will call the right method. */

    assert( const_1.value() == 1 );
    assert( const_2.value() == 2 );
    assert( sum_0.value() == 0 );
    assert( sum_3.value() == 3 );
    assert( prod_4.value() == 4 );
    assert( prod_6.value() == 6 );
    assert( prod_40.value() == 40 );
    assert( sum_9.value() == 9 );

    /* However, the above results should already convince us that
     * dynamic dispatch works as expected: the results depend on the
     * ability of ‹sum::value› and ‹product::value› to call correct
     * versions of the ‹value› method on their children, even though
     * the «static types» of the references stored in ‹operation›
     * are ‹const node›. We can however explore the behaviour in a
     * bit more detail. */

    const node &sum_0_ref = sum_0, &prod_6_ref = prod_6;

    /* Now the «static type» of ‹sum_0_ref› is ‹const node &›, but
     * the «dynamic type» of the value to which it refers is ‹sum›,
     * and for ‹prod_6_ref› the static type is ‹const node &› and
     * dynamic is ‹product›. */

    assert( sum_0_ref.value() == 0 );
    assert( prod_6_ref.value() == 6 );

    /* Let us also check the behaviour of ‹left› and ‹right›. */

    assert( sum_0.left().value()  ==  1 );
    assert( sum_0.right().value() == -1 );

    /* The «static type» through which we call ‹left› and ‹right›
     * does not matter, because neither ‹product› nor ‹sum› provide
     * a different implementation of the method. */

    const operation &op = sum_0;
    assert( op.left().value() == 1 );
    assert( op.right().value() == -1 );

    /* The final thing to check is the ‹static_type› and
     * ‹dynamic_type› methods. By now, we should have a decent
     * understanding of what to expect. */

    /* Please note that ‹sum_0› and ‹sum_0_ref› refer to the «same
     * instance» and hence they have the same «dynamic type», even
     * though their «static types» differ. */

    assert( sum_0.dynamic_type() == type::sum );
    assert( sum_0_ref.dynamic_type() == type::sum );

    assert( sum_0.static_type() == type::sum );

    try { sum_0_ref.static_type(); assert( false ); }
    catch ( const std::logic_error & ) {}

    /* And the same is true about ‹prod_6› and ‹prod_6_ref›. */

    assert( prod_6.dynamic_type() == type::product );
    assert( prod_6_ref.dynamic_type() == type::product );
    assert( prod_6.static_type() == type::product );

    try { prod_6_ref.static_type(); assert( false ); }
    catch ( const std::logic_error & ) {}
}
