#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

/* Given a number ‹n› and a base ‹b›, find all numbers whose digits
 * (in base ‹b›) are a permutation of the digits of ‹n› and return
 * them as a vector of integers. Each such number should appear
 * exactly once.
 *
 * Examples:
 *
 *     (125)₁₀ → { 125, 152, 215, 251, 512, 521 }
 *     (1f1)₁₆ → { (1f1)₁₆, (f11)₁₆, (11f)₁₆ }
 *     (20)₁₀  → { 20, 2 } */

std::vector< unsigned > permute_digits( unsigned n, int base );

std::vector< unsigned > to_digits( unsigned n, int base, int fill = 0 )
{
    std::vector< unsigned > ds;

    while ( n > 0 || fill > 0 )
    {
        ds.push_back( n % base );
        n /= base;
        -- fill;
    }

    return ds;
}

int main()
{
    auto check = [&]( unsigned n, int base,
                      const std::vector< unsigned > &expect )
    {
        auto got = permute_digits( n, base );
        std::set< unsigned > uniq( got.begin(), got.end() );

        assert( got.size() == expect.size() );
        assert( got.size() == uniq.size() );

        auto n_digits = to_digits( n, base );
        std::sort( n_digits.begin(), n_digits.end() );

        for ( unsigned p : got )
        {
            auto p_digits = to_digits( p, base, n_digits.size() );
            std::sort( p_digits.begin(), p_digits.end() );
            assert( n_digits == p_digits );
        }
    };

    check( 1234, 10,
           std::vector{ 4321u, 4312u, 4231u, 4213u, 4132u, 4123u,
                        3421u, 3412u, 3241u, 3214u, 3142u, 3124u,
                        2431u, 2413u, 2341u, 2314u, 2143u, 2134u,
                        1432u, 1423u, 1342u, 1324u, 1243u, 1234u } );

    check( 903, 10, std::vector{ 309u, 390u, 39u, 93u, 930u, 903u } );
    check( 0b1010, 2, std::vector{ 0b1010u, 0b1001u, 0b1100u,
                                   0b11u, 0b110u, 0b101u } );
    check( 0x1f1, 16, std::vector{ 0x11fu, 0xf11u, 0x1f1u } );

    return 0;
}
