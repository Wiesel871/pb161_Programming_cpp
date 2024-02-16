using _pub_::token;
using _pub_::insufficient_space;

struct tiny_vector
{
    std::array< uint8_t, 32 > _mem;
    int _count = 0;

    token *slot( int i )
    {
        assert( i >= 0 );
        assert( i < _count );
        return reinterpret_cast< token * >( _mem.begin() ) + i;
    }

    const token *slot( int i ) const
    {
        return reinterpret_cast< const token * >( _mem.begin() ) + i;
    }

    const token &front() const { return *slot( 0 ); }
    const token &back() const { return *slot( _count - 1 ); }

    token &front() { return *slot( 0 ); }
    token &back() { return *slot( _count - 1 ); }

    ~tiny_vector()
    {
        while ( _count )
            erase( _count - 1 );
    }

    void erase( int idx )
    {
        std::destroy_at( slot( idx ) );

        for ( int i = idx; i < _count - 1; ++i )
        {
            std::uninitialized_move_n( slot( i + 1 ), 1, slot( i ) );
            std::destroy_at( slot( i + 1 ) );
        }

        -- _count;
    }

    void insert( int idx, token &&v )
    {
        const unsigned count = _count;

        if ( count == _mem.size() / sizeof( token ) )
            throw insufficient_space();

        ++ _count;

        for ( int i = _count - 1; i > idx; --i )
        {
            std::uninitialized_move_n( slot( i - 1 ), 1, slot( i ) );
            std::destroy_at( slot( i - 1 ) );
        }

        std::uninitialized_move_n( &v, 1, slot( idx ) );
    }
};
