bool table( bool a, bool b, bool c )
{
    return ( a && !c ) || ( !a && !b && !c );
}

bool bit( auto word, int pos )
{
    const int bitsize = 8 * sizeof( word );
    const decltype( word ) one = 1;

    if ( pos < 0 || pos >= bitsize )
        return false;
    else
        return word & one << pos;
}

auto cellular_step( auto word )
{
    const int bitsize = 8 * sizeof( word );
    decltype( word ) result = 0, new_bit;

    for ( int i = 0; i < bitsize; ++i )
    {
         new_bit = table( bit( word, i + 1 ),
                          bit( word, i ),
                          bit( word, i - 1 ) );
         result |= new_bit << i;
    }

    return result;
}
