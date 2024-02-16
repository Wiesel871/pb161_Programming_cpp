#include <cassert>

/* In this exercise, we will implement a class that represents an
 * array of nibbles (half-bytes) stored compactly, using a byte
 * vector as backing storage. We will need 3 classes: one to
 * represent reference-like objects: ‹nibble_ref›, another for
 * pointer-like objects: ‹nibble_ptr› and finally the container to
 * hold the nibbles: ‹nibble_vec›. NB. In this exercise, we will
 * «not» consider ‹const›-ness of values stored in the vector.¹ */

/* The ‹nibble_ref› class needs to remember a reference or a pointer
 * to the byte which contains the nibble that we refer to, and
 * whether it is the upper or the lower nibble. With that
 * information (which should be passed to it via a constructor), it
 * needs to provide:
 *
 *  • an «assignment operator» which takes an ‹uint8_t› as an
 *    argument, but only uses the lower half of that argument to
 *    overwrite the pointed-to nibble,
 *  • a «conversion operator» which allows implicit conversion of a
 *    ‹nibble_ref› to an ‹uint8_t›. */

class nibble_ref; /* reference implementation: 17 lines */

/* The ‹nibble_ptr› class works as a pointer. «Dereferencing» a
 * ‹nibble_ptr› should result in a ‹nibble_ref›. To make
 * ‹nibble_ptr› more useful, it should also have:
 *
 *  • a pre-increment operator, which shifts the pointer to the next
 *    nibble in memory. That is, if it points at a lower nibble,
 *    after ‹++x›, it should point to an «upper half» of the «same»
 *    byte, and after another ‹++x›, it should point to the «lower
 *    half» of the «next» byte,
 *  • an «equality comparison» operator, which checks whether two
 *    ‹nibble_ptr› instances point to the same place in memory. */

class nibble_ptr; /* reference implementation: 18 lines */

/* And finally the ‹nibble_vec›: this class should provide 4
 * methods:
 *
 *  • ‹push_back›, which adds a nibble at the end,
 *  • ‹begin›, which returns a ‹nibble_ptr› to the first stored
 *    nibble (lower half of the first byte),
 *  • ‹end›, which returns a ‹nibble_ptr› «past» the last stored
 *    nibble (i.e. the first nibble that is not in the container),
 *    and finally
 *  • ‹get( i )› which returns a ‹nibble_ref› to the ‹i›-th nibble.
 */

class nibble_vec; /* reference implementation: 16 lines */

/* ¹ In particular, obtaining a pointer (e.g. by using ‹begin›) will
 *   allow you to change the value that it points to, even if
 *   ‹nibble_vec› itself was marked ‹const›. */

int main()
{
    /* Create a vector and put in a single value; we did not define
     * how should an empty vector behave. */
    nibble_vec v; v.push_back( 4 );

    /* Check that we can access the stored value through ‹begin›. */

    auto ptr = v.begin();
    assert( *ptr == 4 );

    /* And that bumping the ‹ptr› gives us ‹end›. */
    ++ ptr;
    assert( ptr == v.end() );

    /* Add another time and check that things still work okay. */

    v.push_back( 5 ); ptr = v.begin();
    assert( *ptr == 4 ); ++ ptr;
    assert( *ptr == 5 ); ++ ptr;
    assert( ptr == v.end() );

    /* And another, because this is the first time we overflow into
     * the next byte. Since ‹nibble_vec› may need to re-allocate
     * storage, the ‹ptr› is invalid after the ‹push_back›! */

    v.push_back( 6 ); ptr = v.begin();
    assert( *ptr == 4 ); ++ ptr;
    assert( *ptr == 5 ); ++ ptr;
    assert( *ptr == 6 ); ++ ptr;
    assert( ptr == v.end() );

    /* And one last time. */

    v.push_back( 0 ); ptr = v.begin();
    assert( *ptr == 4 ); ++ ptr;
    assert( *ptr == 5 ); ++ ptr;
    assert( *ptr == 6 ); ++ ptr;
    assert( *ptr == 0 ); ++ ptr;
    assert( ptr == v.end() );

    /* Let's check that indexing works. */
    assert( v.get( 0 ) == 4 );
    assert( v.get( 1 ) == 5 );
    assert( v.get( 2 ) == 6 );
    assert( v.get( 3 ) == 0 );

    /* And finally assignment in ‹nibble_ref›. An even index first. */

    v.get( 0 ) = 5;
    assert( v.get( 0 ) == 5 );
    assert( v.get( 1 ) == 5 );
    assert( v.get( 2 ) == 6 );
    assert( v.get( 3 ) == 0 );

    /* Odd indices. */

    v.get( 1 ) = 1;
    assert( v.get( 0 ) == 5 );
    assert( v.get( 1 ) == 1 );
    assert( v.get( 2 ) == 6 );
    assert( v.get( 3 ) == 0 );

    /* And finally the same with the other byte. */

    v.get( 2 ) = 3;
    assert( v.get( 0 ) == 5 );
    assert( v.get( 1 ) == 1 );
    assert( v.get( 2 ) == 3 );
    assert( v.get( 3 ) == 0 );

    v.get( 3 ) = 1;
    assert( v.get( 0 ) == 5 );
    assert( v.get( 1 ) == 1 );
    assert( v.get( 2 ) == 3 );
    assert( v.get( 3 ) == 1 );

    return 0;
}
