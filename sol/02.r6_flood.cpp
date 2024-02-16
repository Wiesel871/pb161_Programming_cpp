using _pub_::grid;

int flood( const grid &pixels, int width,
           int x0, int y0, bool fill )
{
    grid work = pixels;
    int count = 0;
    int height = pixels.size() / width;
    std::queue< std::pair< int, int > > todo;

    if ( pixels.size() % width )
        ++ height;

    while ( static_cast< int >( work.size() ) < width * height )
        work.push_back( false );

    auto flip = [&]( int x, int y )
    {
        int idx = y * width + x;

        if ( x >= 0 && x < width  &&
             y >= 0 && y < height && work[ idx ] != fill )
        {
            todo.emplace( x, y );
            work[ idx ] = fill;
            ++ count;
        }
    };

    flip( x0, y0 );

    while ( !todo.empty() )
    {
        auto [ x, y ] = todo.front();
        todo.pop();

        for ( int dx : { -1, 0, 1 } )
            for ( int dy : { -1, 0, 1 } )
                if ( dx || dy )
                    flip( x + dx, y + dy );
    }

    return count;
}

