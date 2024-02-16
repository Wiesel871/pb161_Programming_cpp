#include <cstddef> // byte
#include <cassert>

/* Implement 2 classes, ‹bitptr› and ‹const_bitptr›, which provide
 * access to a single (mutable or constant) bit. Instances of these
 * classes should behave as pointers in principle, though we don't
 * yet have tools to make them behave this way syntactically (that
 * comes next week). In the meantime, let's use the following
 * interface:
 *
 *  • ‹bool get()› – read the pointed-to bit,
 *  • ‹void set( bool )› – write the same,
 *  • ‹void advance()› – move to the next bit,
 *  • ‹void advance( int )› – move by a given number of bits,
 *  • ‹bool valid()› – is the pointer valid?
 *
 * A default-constructed ‹bitptr› is not valid. Moving an invalid
 * ‹bitptr› results in another invalid ‹bitptr›. Otherwise, a
 * ‹bitptr› is constructed from a ‹std::byte› pointer and an ‹int› with
 * value between 0 and 7 (with 0 being the least-significant bit). A
 * ‹bitptr› constructed this way is always considered valid,
 * regardless of the value of the ‹std::byte› pointer passed to it. */

class bitptr;
class const_bitptr;

int main()
{
    // clang-tidy: -cppcoreguidelines-pro-type-const-cast

    bitptr x;
    assert( !x.valid() );
    x.advance( 3 );
    assert( !x.valid() );

    std::byte val{ 0b0011'0101 };
    const std::byte cval{ 0b0000'0110 };

    bitptr a( &val, 0 );
    const_bitptr ca( &val, 0 );
    // bitptr b( &c, 0 ); should fail to compile
    const_bitptr cb( &cval, 0 );

    assert( a.valid() );
    assert( a.get() == ca.get() );
    assert( a.get() );
    a.advance();
    assert( !a.get() );
    a.advance( -1 );
    assert( a.get() );
    a.advance( 3 );
    assert( !a.get() );

    bitptr null_a( nullptr, 0 );
    bitptr null_b( nullptr, 7 );
    assert( null_a.valid() );
    assert( null_b.valid() );

    const bitptr c, d( &val, 0 );
    const const_bitptr cd( &val, 0 );
    assert( d.get() );
    assert( cd.get() );
    d.set( true );
    assert( d.get() );
    assert( cd.get() );
    d.set( false );
    assert( !d.get() );
    assert( !cd.get() );

    std::byte array[ 3 ] = { std::byte{ 0 },
                             std::byte{ 2 },
                             std::byte{ 0 } };
    bitptr e( array, 0 );
    assert( !e.get() );
    e.advance( 9 );
    assert( e.get() );
    e.advance();
    e.set( true );
    assert( array[ 1 ] == std::byte{ 6 } );

    return 0;
}
