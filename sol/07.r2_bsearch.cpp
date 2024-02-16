using _pub_::token;

class flat_map
{
    std::vector< std::pair< int, token > > _data;
public:

    std::pair< int, bool > index( int k ) const
    {
        int low = 0, high = _data.size();

        while ( low < high )
        {
            int mid = ( low + high ) / 2;

            if ( k < _data[ mid ].first )
                high = mid;
            else if ( k > _data[ mid ].first )
                low = mid + 1;
            else
                return { mid, true };
        }

        assert( low <= int( _data.size() ) );
        assert( low == int( _data.size() ) || _data[ low ].first != k );
        return { low, false };
    }

    bool contains( int k ) const
    {
        return index( k ).second;
    }

    bool emplace( int k, int v )
    {
        auto [ idx, found ] = index( k );

        if ( !found )
            _data.emplace( _data.begin() + idx, std::move( k ), v );

        return !found;
    }

    int index_or_throw( int k ) const
    {
        auto [ idx, found ] = index( k );
        if ( !found )
            throw std::out_of_range( "indexing flat_map" );
        return idx;
    }

    token &at( int k )
    {
        return _data[ index_or_throw( k ) ].second;
    }

    const token &at( int k ) const
    {
        return _data[ index_or_throw( k ) ].second;
    }
};

