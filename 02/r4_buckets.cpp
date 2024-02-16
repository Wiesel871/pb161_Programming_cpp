#include <utility>
#include <vector>
#include <cassert>

/* Sort stones into buckets, where each bucket covers a range of
 * weights; the range of stone weights to put in each bucket is
 * given in an argument – a vector with one element per bucket, each
 * element a pair of min/max values (inclusive). Assume the bucket
 * ranges do not overlap. Stones are given as a vector of weights.
 * Throw away stones which do not fall into any bucket. Return the
 * weights of individual buckets. */

using bucket = std::pair< int, int >;

std::vector< int > sort( const std::vector< int > &stones,
                         const std::vector< bucket > &buckets );

int main()
{
    std::vector< int > st_1{ 1, 2, 3, 16 },
                       st_2{ 5, 10, 15, 20, 25, 30 },
                       st_3{ 25, 1, 1, 1, 1, 1, 5, 5 };

    std::vector< bucket > bs_1{ { 1, 10 }, { 11, 20 }, { 21, 30 } },
                          bs_2{ { 1, 15 }, { 16, 30 } },
                          bs_3{ { 1, 1 }, { 2, 20 }, { 21, 30 } },
                          bs_4{ { 1, 1 }, { 30, 30 } };

    std::vector< int > r_1_1{ 6, 16, 0 },
                       r_1_2{ 6, 16 },
                       r_1_3{ 1, 21, 0 },
                       r_1_4{ 1, 0 },
                       r_2_1{ 15, 35, 55 },
                       r_2_2{ 30, 75 },
                       r_2_3{ 0, 50, 55 },
                       r_2_4{ 0, 30 },
                       r_3_1{ 15, 0, 25 },
                       r_3_2{ 15, 25 },
                       r_3_3{ 5, 10, 25 },
                       r_3_4{ 5, 0 };

    assert( sort( st_1, bs_1 ) == r_1_1 );
    assert( sort( st_1, bs_2 ) == r_1_2 );
    assert( sort( st_1, bs_3 ) == r_1_3 );
    assert( sort( st_1, bs_4 ) == r_1_4 );
    assert( sort( st_2, bs_1 ) == r_2_1 );
    assert( sort( st_2, bs_2 ) == r_2_2 );
    assert( sort( st_2, bs_3 ) == r_2_3 );
    assert( sort( st_2, bs_4 ) == r_2_4 );
    assert( sort( st_3, bs_1 ) == r_3_1 );
    assert( sort( st_3, bs_2 ) == r_3_2 );
    assert( sort( st_3, bs_3 ) == r_3_3 );
    assert( sort( st_3, bs_4 ) == r_3_4 );

    return 0;
}
