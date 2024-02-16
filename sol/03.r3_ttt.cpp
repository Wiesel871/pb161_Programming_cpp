struct tictactoe
{
    int player = -1;
    std::array< int, 9 > board{};

    int index( int x, int y ) const { return x * 3 + y; }

    int read( int x, int y ) const
    {
        return board[ index( x, y ) ];
    }

    void play( int x, int y )
    {
        board[ index( x, y ) ] = player;
        player *= -1;
    }

    int all_of( int x, int y, int dx, int dy ) const
    {
        int w = read( x, y );

        for ( int i = 0; i < 3; ++i )
            if ( w != read( x + dx * i, y + dy * i ) )
                return 0;

        return w;
    }

    int winner() const
    {
        for ( int i = 0; i < 3; ++i )
        {
            if ( int w = all_of( i, 0, 0, 1 ); w != 0 )
                return w;
            if ( int w = all_of( 0, i, 1, 0 ); w != 0 )
                return w;
        }

        if ( int w = all_of( 0, 0, 1, 1 ); w != 0 )
            return w;
        if ( int w = all_of( 0, 2, 1, -1 ); w != 0 )
            return w;

        return 0;
    }
};
