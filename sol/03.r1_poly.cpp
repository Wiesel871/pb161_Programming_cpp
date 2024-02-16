struct poly
{
    std::vector< int > cs;

    void set( int p, int c )
    {
        cs.resize( std::max( degree(), p + 1 ), 0 );
        cs[ p ] = c;
    }

    int get( int p ) const
    {
        return p < degree() ? cs[ p ] : 0;
    }

    int degree() const { return cs.size(); }

    static void check_bounds( int64_t v )
    {
        brq::precondition( v >= INT_MIN && v <= INT_MAX );
    }

    poly operator+( const poly &o ) const
    {
        poly rv;

        for ( int i = 0; i < std::max( degree(), o.degree() ); ++i )
        {
            check_bounds( int64_t( get( i ) ) + o.get( i ) );
            rv.set( i, get( i ) + o.get( i ) );
        }

        return rv;
    }

    poly operator*( const poly &o ) const
    {
        poly rv;

        for ( int i = 0; i < degree(); ++i )
            for ( int j = 0; j < o.degree(); ++j )
            {
                check_bounds( int64_t( get( i ) ) * o.get( j ) );
                check_bounds( rv.get( i + j ) + int64_t( get( i ) ) * o.get( j ) );
                rv.set( i + j,
                        rv.get( i + j ) + get( i ) * o.get( j ) );
            }

        return rv;
    }

    bool operator==( const poly &o ) const
    {
        for ( int i = 0; i < std::max( degree(), o.degree() ); ++i )
            if ( get( i ) != o.get( i ) )
                return false;
        return true;
    }
};

