long phi( long n )
{
    long r = n;
    long p = 2;

    while ( p <= n )
    {
        if ( n % p == 0 )
        {
            r *= p - 1;
            r /= p;
        }

        while ( n % p == 0 )
            n /= p;

        ++ p;
    }

    return r;
}
