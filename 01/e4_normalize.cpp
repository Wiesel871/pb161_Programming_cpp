#include <cassert>

/* Write a function to normalize a fraction, that is, find the
 * greatest common divisor of the numerator and denominator and
 * divide it out. Both numbers are given as in/out parameters. */

// void normalize( … )

int main()
{
    int p = 1, q = 1;
    normalize( p, q );
    assert( p == 1 && q == 1 );

    p = 2, q = 4;
    normalize( p, q );
    assert( p == 1 && q == 2 );

    p = 60, q = 12;
    normalize( p, q );
    assert( p == 5 && q == 1 );

    p = 60, q = 24;
    normalize( p, q );
    assert( p == 5 && q == 2 );

    return 0;
}
