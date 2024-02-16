#include <memory>
#include <cassert>

/* In this example program, we will look at using shared pointers
 * and operator overloading to get a nicer version of our expression
 * examples, this time with sub-structure sharing: that is, doing
 * something like ‹a + a› will not duplicate the sub-expression ‹a›.
 */

/* Like in week 7, we will define an abstract base class to
 * represent the nodes of the expression tree. */

struct expr_base
{
    virtual int eval() const = 0;
    virtual ~expr_base() = default;
};

/* Since we will use (shared) pointers to ‹expr_base› quite often,
 * we can save ourselves some typing by defining a convenient type
 * alias: ‹expr_ptr› sounds like a reasonable name. */

using expr_ptr = std::shared_ptr< expr_base >;

/* We will have two implementations of ‹expr_base›: one for constant
 * values (nothing much to see here), */

struct expr_const : expr_base
{
    const int value;
    expr_const( int v ) : value( v ) {}
    int eval() const override { return value; }
};

/* and another for operator nodes. Those are more interesting,
 * because they need to hold references to the sub-expressions,
 * which are represented as shared pointers. */

struct expr_op : expr_base
{
    enum op_t { add, mul } op;
    expr_ptr left, right;
    expr_op( op_t op, expr_ptr l, expr_ptr r )
        : op( op ), left( l ), right( r )
    {}

    int eval() const override
    {
        if ( op == add ) return left->eval() + right->eval();
        if ( op == mul ) return left->eval() * right->eval();
        assert( false );
    }
};

/* In principle, we could directly overload operators on ‹expr_ptr›,
 * but we would like to maintain the illusion that expressions are
 * values. For that reason, we will implement a thin wrapper that
 * provides a more natural interface (and also takes care of
 * operator overloading). Again, the ‹expr› class essentially
 * provides Java-like object semantics -- which is quite reasonable
 * for immutable objects like our expression trees here. */

struct expr
{
    expr_ptr ptr;
    expr( int v ) : ptr( std::make_shared< expr_const >( v ) ) {}
    expr( expr_ptr e ) : ptr( e ) {}
    int eval() const { return ptr->eval(); }
};

/* The overloaded operators simply construct a new node (of type
 * ‹expr_op› and wrap it up in an ‹expr› instance. */

expr operator+( expr a, expr b )
{
    return { std::make_shared< expr_op >( expr_op::add,
                                          a.ptr, b.ptr ) };
}

expr operator*( expr a, expr b )
{
    return { std::make_shared< expr_op >( expr_op::mul,
                                          a.ptr, b.ptr ) };
}

int main() /* demo */
{
    expr a( 3 ), b( 7 ), c( 2 );
    expr ab = a + b;
    expr bc = b * c;
    expr abc = a + b * c;

    assert( a.eval() == 3 );
    assert( b.eval() == 7 );
    assert( ab.eval() == 10 );
    assert( bc.eval() == 14 );
    assert( abc.eval() == 17 );
}
