using _pub_::choice;

auto select( const auto &l, const auto &r, auto choose )
{
    using type_l = std::decay_t< decltype( *l.begin() ) >;
    using type_r = std::decay_t< decltype( *r.begin() ) >;

    std::vector< std::variant< type_l, type_r > > out;

    auto it_l = l.begin();
    auto it_r = r.begin();

    while ( it_l != l.end() && it_r != r.end() )
    {
        switch ( choose( *it_l, *it_r ) )
        {
            case choice::left: out.emplace_back( *it_l ); break;
            case choice::right: out.emplace_back( *it_r ); break;
        }

        ++ it_l;
        ++ it_r;
    }

    return out;
}

auto project( const auto &seq, auto proj )
{
    using type = std::decay_t< decltype( *proj( &*seq.begin() ) ) >;
    std::vector< type > out;

    for ( const auto &x : seq )
        if ( auto ptr = proj( &x ) )
            out.push_back( *ptr );

    return out;
}

auto left( const auto &seq )
{
    return project( seq, []( auto *x ) { return std::get_if< 0 >( x ); } );
}

auto right( const auto &seq )
{
    return project( seq, []( auto *x ) { return std::get_if< 1 >( x ); } );
}

auto map( const auto &seq, auto left, auto right )
{
    using common_t = decltype( left( std::get< 0 >( *seq.begin() ) ) );
    std::vector< std::decay_t< common_t > > out;

    for ( const auto &x : seq )
        if ( auto ptr = std::get_if< 0 >( &x ) )
            out.push_back( left( *ptr ) );
        else
            out.push_back( right( *std::get_if< 1 >( &x ) ) );

    return out;
}
