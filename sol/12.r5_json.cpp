using _pub_::str_dict;

std::string escape( const std::string &in )
{
    std::ostringstream ostr;

    for ( char c : in )
    {
        if ( c == '\\' || c == '"' )
            ostr << "\\";
        ostr << c;
    }

    return ostr.str();
}

std::string to_json( const str_dict &dict )
{
    std::ostringstream ostr;

    ostr << "{";
    bool comma = false;

    for ( auto [ k, v ] : dict )
    {
        ostr << ( comma ? ", " : " " );
        comma = true;
        ostr << "\"" << escape( k ) << "\": \"" << escape( v ) << "\"";
    }

    ostr << ( comma ? " }" : "}" );

    return ostr.str();
}
