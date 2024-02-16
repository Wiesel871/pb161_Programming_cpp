int even_parity( std::uint8_t x )
{
    bool r = true;

    for ( ; x != 0; x >>= 1 )
        if ( x & 1 )
            r = !r;

    return r;
}

bool h84_decode( std::uint8_t data, std::uint8_t &out )
{
    bool ok = even_parity( data ) &&
              even_parity( data & 0b01010101 ) &&
              even_parity( data & 0b00110011 ) &&
              even_parity( data & 0b00001111 );

    if ( ok )
        out = ( data & 7 ) | ( data & 16 ) >> 1;

    return ok;
}

