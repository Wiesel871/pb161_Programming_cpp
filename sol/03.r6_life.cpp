using _pub_::grid;

bool updated( int x, int y, const grid &cells )
{
    int count = 0;
    bool alive = cells.count( { x, y } );

    for ( int dx : { -1, 0, 1 } )
        for ( int dy : { -1, 0, 1 } )
            if ( dx || dy )
                count += cells.count( { x + dx, y + dy } );

    return alive ? count == 2 || count == 3 : count == 3;
}

grid life( const grid &cells, int n )
{
    if ( n == 0 )
        return cells;

    grid todo, ngen;

    for ( auto [ x, y ] : cells )
        for ( int dx : { -1, 0, 1 } )
            for ( int dy : { -1, 0, 1 } )
                todo.emplace( x + dx, y + dy );

    for ( auto [ x, y ] : todo )
        if ( updated( x, y, cells ) )
            ngen.emplace( x, y );

    return life( ngen , n - 1 );
}

