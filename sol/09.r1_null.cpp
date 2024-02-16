auto filter( const auto &seq )
{
    std::vector< std::decay_t< decltype( **seq.begin() ) > > out;

    for ( const auto &x : seq )
        if ( x.has_value() )
            out.push_back( *x );

    return out;
}

auto zip( const auto &seq_a, const auto &seq_b, auto f )
{
    using res_t = decltype( f( **seq_a.begin(), **seq_b.begin() ) );
    std::vector< std::optional< std::decay_t< res_t > > > out;

    auto it_a = seq_a.begin();
    auto it_b = seq_b.begin();

    while ( it_a != seq_a.end() && it_b != seq_b.end() )
    {
        if ( it_a->has_value() && it_b->has_value() )
            out.push_back( f( **it_a, **it_b ) );
        else
            out.push_back( std::nullopt );

        ++ it_a;
        ++ it_b;
    }

    return out;
}

auto join( const auto &seq_a, const auto &seq_b, auto f )
{
    using type_a = std::decay_t< decltype( **seq_a.begin() ) >;
    using type_b = std::decay_t< decltype( **seq_b.begin() ) >;
    std::vector< std::tuple< type_a, type_b > > out;

    for ( const auto &a : seq_a )
        for ( const auto &b : seq_b )
            if ( a.has_value() && b.has_value() && f( *a, *b ) )
                out.emplace_back( *a, *b );

    return out;
}
