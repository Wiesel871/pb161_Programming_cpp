#include <cassert>

/* In this example, we will implement a class which behaves like a
 * nullable reference to an integer. We will pretend we are in Java
 * and will throw an exception when the user attempts to use a null
 * reference. */

/* We first define the type which we will use to indicate an attempt
 * to use an invalid (null) reference. */

class null_pointer_exception {};

/* Now for the reference-like class itself. We need two basic
 * ingredients to provide simple reference-like behaviours: we need
 * to be able to (implicitly) convert a value of type ‹maybe_ref› to
 * a value of type ‹int›. The other part is the ability to «assign»
 * new values of type ‹int› to the referred-to variable, via
 * instances of the class ‹maybe_ref›. */

class maybe_ref
{
    /* We hold a pointer internally, since real references in C++
     * cannot be null. */

    int *_ptr = nullptr;

    /* We will also define a helper (internal, private) method which
     * checks whether the reference is valid. If the reference is
     * null, it throws the above exception. */

    void _check() const
    {
        if ( !_ptr )
            throw null_pointer_exception();
    }

public:

    /* Constructors: the default-constructed ‹maybe_ref› instances
     * are nulls, they have nowhere to point. Like real references
     * in C++, we will allow ‹maybe_ref› to be initialized to point
     * to an existing value. We take the argument by reference and
     * convert that reference into a pointer by using the unary ‹&›
     * operator, in order to store it in ‹_ptr›. */

    maybe_ref() = default;
    maybe_ref( int &i ) : _ptr( &i ) {}

    /* As mentioned earlier, we need to be able to (implicitly)
     * convert ‹maybe_ref› instances into integers. The syntax to do
     * that is ‹operator type›, without mentioning the return type
     * (in this case, the return type is given by the name of the
     * operator, i.e. ‹int› here). It is also possible to have
     * reference conversion operators, by writing e.g. ‹operator
     * const int &()›. However, we don't need one of those here
     * because ‹int› is small, and we can't have both since that
     * would cause a lot of ambiguity. */

    operator int() const
    {
        _check();
        return *_ptr;
    }

    /* The final part is assignment: as you surely remember,
     * ‹operator=› should return a reference to the assigned-to
     * instance. It usually takes a ‹const› reference as an
     * argument, but again we do not need to do that here. Below in
     * the demo, we will point out where the assignment operator
     * comes into play. */

    maybe_ref &operator=( int v )
    {
        _check();
        *_ptr = v;
        return *this;
    }
};

int main() /* demo */
{
    int i = 7;

    /* When initializing built-in references, we use ‹int &i_ref =
     * i›. We can do the same with ‹maybe_ref›, but we need to keep
     * in mind that this syntax calls the ‹maybe_ref( int )›
     * constructor, «not» the assignment operator. */

    maybe_ref i_ref = i;

    /* Let us check that the reference behaves as expected. */

    assert( i_ref == 7 ); /* uses conversion to ‹int› */
    i_ref = 3;            /* uses the assignment operator */
    assert( i_ref == 3 ); /* conversion to ‹int› again */

    /* Check that the original variable has changed too. */

    assert( i == 3 );

    /* Let's also check that null references behave as expected. */

    bool caught = false;
    maybe_ref null;

    /* Comparison will try to convert the reference to ‹int›, but
     * that will fail in ‹_check› with an exception. */

    try { assert( null == 7 ); }
    catch ( const null_pointer_exception & ) { caught = true; }

    /* Make sure that the exception was thrown and caught. */

    assert( caught );
    caught = false;

    /* Same but with assignment into the null referenc. */

    try { null = 2; }
    catch ( const null_pointer_exception & ) { caught = true; }

    assert( caught );
}
