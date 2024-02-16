auto accumulate( auto f, const std::vector< int > &vec )
{
    int sum = 0;

    for ( int x : vec )
        sum += f( x );

    return sum;
};
