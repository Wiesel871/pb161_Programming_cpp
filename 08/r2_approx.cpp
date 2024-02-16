#include <cmath>
#include <cassert>

/* Remember ‹fib.cpp›? We can do a bit better. Let's decompose our
 * golden() function differently this time. */

/* The ‹approx› function is a higher-order one. What it does is it
 * calls f() repeatedly to improve the current estimate, until the
 * estimates are sufficiently close to each other (closer than the
 * given precision). The ‹init› argument is our initial estimate of
 * the result. */

double approx( auto f, double init, double prec );

/* Use ‹approx› to compute the golden mean. Note that you don't need
 * to use the previous estimate in your improvement function. Use
 * by-reference captures to keep state between iterations if you
 * need some. */

double golden( double prec );

int main()
{
    /* The Babylonian (Heron) method to compute square roots. Please
     * take note, you may find it helpful later. This is how ‹approx› is
     * supposed to be used. */

    auto babylonian = []( double n, double prec )
    {
        auto improve = [=]( double last )
        {
            double next = n / last;
            return ( last + next ) / 2;
        };

        return approx( improve, 1, prec );
    };

    for ( int n = 2; n < 20; ++n )
        for ( double p = .1 ; p > 1e-10; p /= 10 )
            assert( std::fabs( babylonian( n, p ) - std::sqrt( n ) ) <= p );

    double actual = ( 1 + std::sqrt( 5.L ) ) / 2;
    for ( double p = .1 ; p > 1e-10; p /= 10 )
        assert( std::fabs( golden( p ) - actual ) <= p );

    return 0;

    // clang-tidy: -cert-flp30-c,-clang-analyzer-security.FloatLoopCounter
}
