bool table( std::uint8_t op, bool a, bool b, bool c )
{
    int shift = ( a ? 4 : 0 ) + ( b ? 2 : 0 ) + ( c ? 1 : 0 );
    return op & 1 << shift;
}

auto bitwise( std::uint8_t op, auto a, auto b, auto c )
{
    decltype( a ) r = 0;

    for ( decltype( a ) mask = 1; mask != 0; mask <<= 1 )
        if ( table( op, a & mask, b & mask, c & mask ) )
            r |= mask;

    return r;
}
