int factorial( int n )
{
    int r = 1;

    for ( ; n > 0; n-- )
        r *= n;

    return r;
}
