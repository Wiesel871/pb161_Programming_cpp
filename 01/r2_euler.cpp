#include <cassert>

/* This is a straightforward math exercise. Implement Euler's [φ],
 * for instance using the product formula ⟦φ(n) = nΠ(1 - 1/p)⟧ where
 * the product is over all distinct prime divisors of n. You may
 * need to take care to compute the result exactly. */

long phi( long n ); /* ref: 21 lines */

int main()
{
    assert( phi( 1 )   ==   1 );
    assert( phi( 2 )   ==   1 );
    assert( phi( 3 )   ==   2 );
    assert( phi( 4 )   ==   2 );
    assert( phi( 5 )   ==   4 );
    assert( phi( 6 )   ==   2 );
    assert( phi( 12 )  ==   4 );
    assert( phi( 24 )  ==   8 );
    assert( phi( 36 )  ==  12 );
    assert( phi( 101 ) == 100 );
    assert( phi( 112 ) ==  48 );
    assert( phi( 139 ) == 138 );
    assert( phi( 140 ) ==  48 );

    /* a few big cases */

    assert( phi(  60000 ) ==  16000 );
    assert( phi(  65535 ) ==  32768 );
    assert( phi(  65536 ) ==  32768 );
    assert( phi(  65536 ) ==  32768 );
    assert( phi( 139761 ) ==  91104 );
    assert( phi( 139761 ) ==  91104 );
    assert( phi( 611953 ) == 611952 );
    assert( phi( 611954 ) == 262260 );

    return 0;
}
