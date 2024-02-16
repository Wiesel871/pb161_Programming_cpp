void join( auto a, auto b, unsigned i, unsigned j, auto &out )
{
    auto a_cmp = [&]( const auto &u, const auto &v )
    {
        return u[ i ] < v[ i ];
    };

    auto b_cmp = [&]( const auto &u, const auto &v )
    {
        return u[ j ] < v[ j ];
    };

    std::sort( a.begin(), a.end(), a_cmp );
    std::sort( b.begin(), b.end(), b_cmp );

    auto it_a = a.begin();
    auto it_b = b.begin();

    for ( ; it_a != a.end(); ++it_a )
    {

        while ( it_b != b.end() && ( *it_b )[ j ] < ( *it_a )[ i ] )
            ++it_b;

        for ( auto it = it_b;
              it != b.end() && ( *it )[ j ] == ( *it_a )[ i ];
              ++it )
        {
            auto &out_row = out.emplace_back();

            unsigned col = 0;
            unsigned n = it_a->size();
            unsigned m = it->size();

            for ( col = 0; col < n; ++ col )
                out_row[ col ] = ( *it_a )[ col ];
            for ( ; col < n + m - 1; ++ col )
                out_row[ col ] = ( *it )[ col - n < j ? col - n : col - n + 1 ];
        }
    }
}

