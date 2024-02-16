bool recurse( int pos, std::vector< bool > &visited,
              const std::vector< int > &jumps )
{
    if ( pos == int( jumps.size() ) )
    {
        int cnt = std::count( visited.begin(), visited.end(),
                              true );
        return int( jumps.size() ) == cnt;
    }

    if ( pos < 0 || pos >= int( visited.size() ) || visited[ pos ] )
        return false;

    visited[ pos ] = true;
    bool won = recurse( pos - jumps[ pos ], visited, jumps ) ||
               recurse( pos + jumps[ pos ], visited, jumps );
    visited[ pos ] = false;
    return won;
}

bool solve( std::vector< int > jumps )
{
    std::vector< bool > visited( jumps.size(), false );
    return recurse( 0, visited, jumps );
}
