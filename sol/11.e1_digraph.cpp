struct strmap
{
    std::map< std::string, int > m;

    int operator[]( std::string s ) const
    {
        return m.contains( s ) ? m.find( s )->second : 0;
    }

    void add( std::string s )
    {
        m[ s ] ++;
    }
};

strmap digraph_freq( std::string_view s )
{
    strmap m;

    for ( size_t i = 0; i < s.size() - 1; ++i )
        if ( std::isalpha( s[ i ] ) && std::isalpha( s[ i + 1 ] ) )
            m.add( std::string( s.substr( i, 2 ) ) );

    return m;
}

