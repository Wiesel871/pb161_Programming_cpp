#include <cassert>

/* † Implement ‹tiny_vector›, a class which works like a vector, but
 * instead of allocating memory dynamically, it uses a fixed-size
 * buffer (32 bytes) which is part of the object itself (use e.g. an
 * ‹std::array› of bytes). Like earlier, we will use ‹token› as the
 * value type. Provide the following methods:
 *
 *  • ‹insert› (take an index and an rvalue reference),
 *  • ‹erase› (take an index),
 *  • ‹back› and ‹front›, with appropriate return types.
 *
 * In this exercise (unlike in most others), you are allowed to use
 * ‹reinterpret_cast›. */

class token
{
    int _value;
    bool _robbed = false;
public:
    static int _count;

    token( int i ) : _value( i ) { ++ _count; }
    ~token() { -- _count; }

    token( const token & ) = delete;
    token( token &&o ) : _value( o._value )
    {
        ++ _count;
        assert( !o._robbed );
        o._robbed = true;
    }

    token &operator=( const token & ) = delete;
    token &operator=( token &&o )
    {
        assert( !o._robbed );
        _value = o._value;
        _robbed = o._robbed;
        o._robbed = true;
        return *this;
    }

    token &operator=( int v )
    {
        _value = v;
        _robbed = false;
        return *this;
    }

    bool operator==( int v ) const
    {
        assert( !_robbed );
        return _value == v;
    }
};

/* Throw this if ‹insert› is attempted but the element wouldn't fit
 * into the buffer. */
class insufficient_space {};

/* Hint: Use ‹uninitialized_*› and ‹destroy(_at)› functions from the
 * ‹memory› header. */

class tiny_vector;

int token::_count = 0;

int main()
{
    // clang-tidy: -cppcoreguidelines-pro-type-reinterpret-cast

    {
        tiny_vector v;

        v.insert( 0, 3 );
        assert( v.front() == 3 );
        assert( v.back() == 3 );
        assert( token::_count == 1 );

        v.insert( 0, 4 );
        assert( v.front() == 4 );
        assert( v.back() == 3 );
        assert( token::_count == 2 );

        v.insert( 2, 5 );
        assert( v.front() == 4 );
        assert( v.back() == 5 );
        assert( token::_count == 3 );

        v.insert( 1, 7 );

        try
        {
            v.insert( 1, 8 );
            assert( false );
        } catch ( insufficient_space ) {};

        v.erase( 1 );

        v.erase( 0 );
        assert( v.front() == 3 );
        assert( v.back() == 5 );
        assert( token::_count == 2 );

        v.front() = 4;
        assert( v.front() == 4 );
        assert( v.back() == 5 );
        assert( token::_count == 2 );

        v.erase( 1 );
        assert( v.front() == 4 );
        assert( v.back() == 4 );
        assert( token::_count == 1 );

        const tiny_vector &cv = v;

        assert( cv.front() == 4 );
        assert( cv.back() == 4 );
        assert( token::_count == 1 );
    }

    assert( token::_count == 0 );

    return 0;
}
