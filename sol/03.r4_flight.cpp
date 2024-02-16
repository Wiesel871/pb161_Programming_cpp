struct flight
{
    using bound_t = std::tuple< double, double >;

    double climb = 0, y = 0;
    int x = 0;
    std::vector< bound_t > bounds{ { -10, 10 } };
    bool flying = true;

    void append( double l, double h )
    {
        brq::precondition( l < h );
        bounds.emplace_back( l, h );
    }

    int size() const { return bounds.size(); }
    bool finished() const { return x == size() - 1; }
    void set_climb( double c ) { climb = c; }

    bool clear() const
    {
        auto [ l, h ] = bounds[ x ];
        return l < y && y < h;
    }

    bool move( int l )
    {
        if ( !flying )
            return false;

        while ( l-- > 0 && x < size() - 1 )
        {
            ++ x;
            y += climb;

            if ( !clear() )
                return flying = false;
        }

        return true;
    }
};

