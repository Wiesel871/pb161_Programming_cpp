using _pub_::bucket;

std::vector< int > sort( const std::vector< int > &stones,
                         const std::vector< bucket > &buckets )
{
    std::vector< int > out( buckets.size(), 0 );

    for ( int s : stones )
        for ( size_t i = 0; i < buckets.size(); ++ i )
        {
            auto [ min, max ] = buckets[ i ];

            if ( s >= min && s <= max )
                out[ i ] += s;
        }

    return out;
}
