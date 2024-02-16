bool rotate_sort( auto &seq, int n )
{
    std::vector< int > rot;

    for ( auto it = seq.begin(); it != seq.end(); std::advance( it, n ) )
    {
        auto end = std::next( it, n );
        auto mid = std::is_sorted_until( it, end );

        if ( mid == end )
            rot.push_back( 0 );
        else if ( std::is_sorted( mid, end ) && *it >= *std::next( it, n - 1 ) )
            rot.push_back( std::distance( it, mid ) );
        else
            return false;
    }

    auto b = seq.begin();

    for ( unsigned i = 0; i < rot.size(); ++i )
        std::rotate( std::next( b, i * n ),
                     std::next( b, i * n + rot[ i ] ),
                     std::next( b, i * n + n ) );

    return true;
}

