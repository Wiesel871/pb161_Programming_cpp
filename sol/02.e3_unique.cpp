std::vector< int > unique( const std::vector< int > &v )
{
    std::vector< int > out;
    std::set< int > seen;

    for ( int x : v )
        if ( !seen.count( x ) )
        {
            out.push_back( x );
            seen.insert( x );
        }

    return out;
}
