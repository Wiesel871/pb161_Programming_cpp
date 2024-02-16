using _pub_::to_digits;

unsigned from_digits( const std::vector< unsigned > &digits, int base )
{
    unsigned r = 0;

    for ( unsigned d : digits )
    {
        r *= base;
        r += d;
    }

    return r;
}

std::vector< unsigned > permute_digits( unsigned n, int base )
{
    std::set< unsigned > r;
    auto digits = to_digits( n, base );
    std::sort( digits.begin(), digits.end() );

    do
        r.insert( from_digits( digits, base ) );
    while ( std::next_permutation( digits.begin(), digits.end() ) );

    return std::vector< unsigned >( r.begin(), r.end() );
}
