inline auto split( const std::string &sv, char delim )
{
    if ( auto offset = sv.find( delim ); offset != sv.npos )
        return std::pair( sv.substr( 0, offset ), sv.substr( offset + 1, sv.npos ) );
    else
        return std::pair( sv, std::string() );
}

class preprocessor
{
    std::set< std::string > defs;
    std::stack< bool > _emit;
public:
    std::string out;

    bool emit() const { return _emit.empty() || _emit.top(); }

    void process( const std::string &line )
    {
        auto [ dir, args ] = split( line, ' ' );

        if ( dir == "#ifdef" )
            _emit.push( defs.count( args ) );
        if ( dir == "#endif" )
            _emit.pop();

        if ( emit() )
        {
            if ( dir == "#define" )
                defs.insert( args );
            if ( dir == "#undef" )
                defs.erase( args );
            if ( dir == "#include" )
                read( args.substr( 1, args.size() - 2 ) );
        }
    }

    void read( const std::string &filename )
    {
        std::ifstream in( filename ); /* NB. Fails quietly. */
        std::string line;

        while ( std::getline( in, line ) )
            if ( !line.empty() && line[ 0 ] == '#' )
                process( line );
            else if ( emit() )
                out += line + "\n";
    }
};

std::string cpp( const std::string &filename )
{
    preprocessor p;
    p.read( filename );
    return p.out;
}
