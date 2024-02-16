using _pub_::walk;
using _pub_::turn;
using _pub_::toggle;

struct program
{
    using command = std::variant< walk, turn, toggle >;
    std::vector< command > commands;

    void append( auto c )
    {
        commands.emplace_back( c );
    }
};

struct grid
{
    using position_t = std::tuple< int, int >;
    int x = 0, y = 0;
    bool horizontal = true;
    std::set< position_t > marked;

    position_t robot() const
    {
        return { x, y };
    }

    bool on_marked() const
    {
        return marked.contains( robot() );
    }

    void exec( toggle t )
    {
        if ( on_marked() )
        {
            if ( !t.sticky )
                marked.erase( robot() );
        }
        else
            marked.insert( robot() );
    }

    void exec( turn )
    {
        horizontal = !horizontal;
    }

    void exec( walk w )
    {
        auto &coord = horizontal ? x : y;
        coord += on_marked() ? w.if_marked : w.if_unmarked;
    }
};

std::tuple< int, int > run( const program &p, grid &g )
{
    for ( auto cmd : p.commands )
        std::visit( [&]( auto c ) { g.exec( c ); }, cmd );

    return { g.x, g.y };
}
