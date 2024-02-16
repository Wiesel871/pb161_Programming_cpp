#include <vector>
#include <cassert>

/* In this demonstration, we will look at overloading functions
 * based on different kinds of references. This will allow us to
 * adapt our functions to the kind of value (and its lifetime) that
 * is passed to them, and to deal with arguments efficiently
 * (without making unnecessary copies). But first, let's define a
 * few type aliases: */

using int_pair   = std::pair< int, int >;
using int_vector = std::vector< int >;
using int_matrix = std::vector< int_vector >;

/* Our goal will be simple enough: write a function which gives
 * access to the first element of any of the above types. In the
 * case of ‹int_matrix›, the element is an entire row, which has
 * some important implications that we will discuss shortly.
 *
 * Our main requirements will be that: 
 *
 *  1. ‹first› should work correctly when we call it on a constant,
 *  2. when called on a mutable value, ‹first( x ) = y› should work
 *     and alter the value ‹x› (i.e. update the first element of
 *     ‹x›).
 *
 * These requirements are seemingly contradictory: if we return a
 * value (or a constant reference), we can satisfy point 1, but we
 * fail point 2. If we return a mutable reference, point 2 will
 * work, but point 1 will fail. Hence we need the result type to be
 * different depending on the argument. This can be achieved by
 * overloading on the argument type.
 *
 * However, we still have one problem: how do we tell apart, using a
 * type, whether the passed value was constant or not? Think about
 * this: if you write a function which accepts a mutable reference,
 * it cannot be called on an argument which is constant: the
 * compiler will complain about the value losing its ‹const›
 * qualifier (if you never encountered this behaviour, try it out;
 * it's important that you understand this).

 * But that means that ‹first( int_pair &arg )› can only be called
 * on mutable arguments, which is exactly what we need. Fortunately
 * for us, if the compiler decides that this particular ‹first›
 * cannot be used (because of missing ‹const›), it will keep looking
 * for some other ‹first› that might work. You hopefully remember
 * that ‹first( const int_pair &arg )› can be called on any value of
 * type ‹int_pair› (without creating a copy). If we provide both,
 * the compiler will use the non-‹const› version if it can, but fall
 * back to the ‹const› one otherwise. And since overloaded functions
 * can differ in their return type, we have our solution: */

int &first(       int_pair &p ) { return p.first; }
int  first( const int_pair &p ) { return p.first; }

/* The case of ‹int_vector› is completely analogous: */

int &first(       int_vector &v ) { return v[ 0 ]; }
int  first( const int_vector &v ) { return v[ 0 ]; }

/* Since in the above cases, the return value was of type ‹int›, we
 * did not bother with returning ‹const› references. But when we
 * look at ‹int_matrix›, the situation has changed: the value which
 * we return is an ‹std::vector›, which could be very expensive to
 * copy. So we will want to avoid that. The first case (mutable
 * argument), stays the same – we already returned a reference in
 * this case. */

int_vector &first( int_matrix &v ) { return v[ 0 ]; }

/* At first glance, the second case would seem straightforward
 * enough – just return a ‹const int_vector &› and be done with it.
 * But there is a catch: what if the argument is a temporary value,
 * which will be destroyed at the end of the current statement? It's
 * not a very good idea to return a reference to a doomed object,
 * since an unwitting caller could get into serious trouble if they
 * store the returned reference – that reference will be invalid on
 * the next line, even though there is no obvious reason for that at
 * the callsite.
 *
 * You perhaps also remember, that the above function, with a
 * mutable reference, cannot be used with a temporary as its
 * argument: like with a constant, the compiler will complain that
 * it cannot bind a temporary to an argument of type ‹int_matrix &›.
 * So is there some kind of a reference that can bind a temporary,
 * but not a constant? Yes, that would be an «rvalue reference»,
 * written ‹int_matrix &&›. If the above candidate fails, the next
 * one the compiler will look at is one with an rvalue reference as
 * its argument. In this case, we know the value is doomed, so we
 * better return a value, not a reference into the doomed matrix.
 * Moreover, since the input matrix is doomed anyway, we can steal
 * the value we are after using ‹std::move› and hence still manage
 * to avoid a copy. */

int_vector first( int_matrix &&v ) { return std::move( v[ 0 ] ); }

/* If both of the above fail, the value must be a constant – in this
 * case, we can safely return a reference into the constant. The
 * argument is not immediately doomed, so it is up to the caller to
 * ensure that if they store the reference, it does not outlive its
 * parent object. */

const int_vector &first( const int_matrix &v )
{
    return v[ 0 ];
}

/* That concludes our quest for a polymorphic accessor. Let's have a
 * look at how it works when we try to use it: */

int main() /* demo */
{
    int_vector v{ 3, 5, 7, 1, 4 };
    assert( first( v ) == 3 );
    first( v ) = 5;
    assert( first( v ) == 5 );

    const int_vector &const_v = v;
    assert( first( const_v ) == 5 );

    int_matrix m{ int_vector{ 1, 2, 3 }, v };
    const int_matrix &const_m = m;

    assert( first( first( m ) ) == 1 );
    first( first( m ) )= 2;

    assert( first( first( const_m ) ) == 2 );
    assert( first( first( int_matrix{ v, v } ) ) == 5 );

    /* What follows is the case where the rvalue-reference overload
     * of ‹first› (the one which handles temporaries) saves us: try
     * to comment the overload out and see what happens on the next
     * 2 lines for yourself. */

    const int_vector &x = first( int_matrix{ v, v } );
    assert( first( x ) == 5 );
}
