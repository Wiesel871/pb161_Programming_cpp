#include <cassert>
#include <array>

/* Implement class ‹lock› which holds a mutex locked as long as it
 * exists. The ‹lock› instance can be moved around. For simplicity,
 * the ‹mutex› itself is immovable. */

class mutex
{
    bool _locked = false;
public:
    ~mutex() { assert( !_locked ); }

    mutex() = default;
    mutex( const mutex & ) = delete;
    mutex( mutex && ) = delete;
    mutex &operator=( const mutex & ) = delete;
    mutex &operator=( mutex && ) = delete;

    void lock() { assert( !_locked ); _locked = true; }
    void unlock() { assert( _locked ); _locked = false; }
    bool locked() const { return _locked; }
};

class lock;

int main()
{
    mutex m;
    std::array< mutex, 4 > m_arr;

    auto with_lock = [&]( lock l )
    {
        assert( m.locked() );
        return l;
    };

    {
        assert( !m.locked() );
        lock l_1( m );
        assert( m.locked() );
        lock l_2 = std::move( l_1 );
        assert( m.locked() );
        lock l_3 = with_lock( std::move( l_2 ) );
        assert( m.locked() );
    }

    assert( !m.locked() );

    {
        std::array< lock, 3 > l_arr{ m_arr[ 0 ], m_arr[ 1 ], m_arr[ 2 ] };
        assert(  m_arr[ 0 ].locked() );
        assert(  m_arr[ 1 ].locked() );
        assert(  m_arr[ 2 ].locked() );
        assert( !m_arr[ 3 ].locked() );

        std::swap( l_arr[ 0 ], l_arr[ 2 ] );
        assert(  m_arr[ 0 ].locked() );
        assert(  m_arr[ 2 ].locked() );
        assert( !m_arr[ 3 ].locked() );

        l_arr[ 1 ] = lock( m );
        assert( m.locked() );
        assert( !m_arr[ 1 ].locked() );
    }

    assert( !m.locked() );
    assert( !m_arr[ 0 ].locked() );
    assert( !m_arr[ 1 ].locked() );
    assert( !m_arr[ 2 ].locked() );

    return 0;
}
