struct array
{
    std::vector< int > vec;

    int get( int i ) const { return vec[ i ]; }
    void append( int x ) { vec.push_back( x ); }

    int partition( int pivot, int low, int high )
    {
        while ( vec[ low ] < pivot ) /* the pivot must be in there */
            ++ low;

        int p_index = low;

        /* shuffle anything < pivot to the front while remembering where
         * (in the second half) we stashed the pivot itself */

        for ( int i = low + 1; i < high; ++i )
        {
            if ( vec[ i ] < pivot )
                std::swap( vec[ low++ ], vec[ i ] );
            if ( vec[ i ] == pivot )
                p_index = i;
        }

        /* put the pivot in its place between the partitions */

        std::swap( vec[ p_index ], vec[ low ] );

        return low;
    }

    void partition( int pivot )
    {
        partition( pivot, 0, vec.size() );
    }

    void sort( int low, int high )
    {
        if ( high - low <= 1 )
            return;

        int pivot = vec[ low ]; /* whatever */
        int p_index = partition( pivot, low, high );
        sort( low, p_index );
        sort( p_index + 1, high );
    }

    void sort()
    {
        sort( 0, vec.size() );
    }
};
