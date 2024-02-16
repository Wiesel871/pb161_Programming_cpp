int zeros( int n, int base, int &order )
{
    int result = 0;

    for ( int current_order = 0; n; current_order ++ )
    {
        if ( n % base == 0 )
        {
            order = current_order;
            ++ result;
        }

        n /= base;
    }

    return result;
}
