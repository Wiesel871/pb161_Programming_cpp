#include <cassert>

/* In this exercise, we will go back to building data structures, in
 * this particular case a simple binary tree. The structure should
 * represent a partitioning of an interval with integer bounds into
 * a set of smaller, non-overlapping intervals. */

/* Implement class ‹segment_map› with the following interface:
 *
 *  • the constructor takes two integers, which represent the limits
 *    of the interval to be segmented,
 *  • a ‹split› operation takes a single integer, which becomes the
 *    start of a new segment, splitting the existing segment in two,
 *  • ‹query›, given an integer ‹n›, returns the bounds of the
 *    segment that contains ‹n›, as an ‹std::pair› of integers.
 *
 * The tree does «not» need to be self-balancing: the order of
 * splits will determine the shape of the tree. */

int main()
{
    segment_map sm( 1, 1000 );
    const segment_map &cm = sm;

    assert( cm.query( 5 ) == std::pair( 1, 1000 ) );

    sm.split( 10 );
    assert( cm.query(  5 ) == std::pair(  1,   10 ) );
    assert( cm.query( 11 ) == std::pair( 10, 1000 ) );

    sm.split( 100 );
    assert( cm.query(   5 ) == std::pair(   1,   10 ) );
    assert( cm.query(  11 ) == std::pair(  10,  100 ) );
    assert( cm.query( 901 ) == std::pair( 100, 1000 ) );

    return 0;
}
