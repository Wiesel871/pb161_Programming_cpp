auto sorted_ranges( const auto &in, int n )
{
    std::vector< std::tuple< int, int > > out;
    auto row_b = in.begin(), row_e = row_b + n;

    while ( row_b != in.end() )
    {
        int len_max = 0;
        int idx_max;
        auto it = row_b;

        while ( it != row_e )
        {
            auto next = std::is_sorted_until( it, row_e );
            int len = std::distance( it, next );

            if ( len > len_max )
            {
                idx_max = std::distance( row_b, it );
                len_max = len;
            }

            it = next;
        }

        out.emplace_back( idx_max, len_max );
        std::advance( row_b, n );
        std::advance( row_e, n );
    }

    return out;
}

