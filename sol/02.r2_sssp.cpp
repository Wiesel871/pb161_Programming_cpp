using _pub_::graph;

std::map< int, int > shortest( const graph &g, int initial )
{
    std::map< int, int > dist;
    std::queue< int > queue;
    queue.push( initial );
    dist[ initial ] = 0;

    while ( !queue.empty() )
    {
        int from = queue.front();
        queue.pop();

        for ( auto to : g.at( from ) )
        {
            if ( dist.count( to ) )
                continue;

            dist[ to ] = dist[ from ] + 1;
            queue.push( to );
        }
    }

    return dist;
}
