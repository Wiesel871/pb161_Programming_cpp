void fibonacci( std::vector< int > &v, int n )
{
    v.clear();

    if ( n > 0 ) v.push_back( 1 );
    if ( n > 1 ) v.push_back( 1 );

    for ( int i = 2; i < n; ++ i )
        v.push_back( v[ i - 1 ] + v[ i - 2 ] );
}

