int mode( const std::vector< int > &in )
{
    std::map< int, int > freq;
    int max_val = 0, max_freq = 0;

    for ( int x : in )
        freq[ x ] ++;

    for ( auto [ v, f ] : freq )
        if ( f > max_freq )
        {
            max_val = v;
            max_freq = f;
        }

    return max_val;
}
