#include <map>
#include <set>
#include <queue>
#include <cassert>

/* † The goal of this demonstration will be to find the shortest
 * distance in an unweighted, directed graph:
 *
 *  1. starting from a fixed (given) vertex,
 *  2. to the nearest vertex with an odd value.
 *
 * The canonical ‘shortest path’ algorithm in this setting is
 * «breadth-first search». The algorithm makes use of two data
 * structures: a «queue» and a «set», which we will represent using
 * the standard C++ containers named, appropriately, ‹std::queue›¹
 * and ‹std::set›.
 *
 * In the previous demonstration, we have represented the graph
 * explicitly using adjacency list encoded as instances of
 * ‹std::vector›. Here, we will take a slightly different approach:
 * we well use ‹std::multimap› – as the name suggests, it is related
 * to ‹std::map› with one crucial difference: it can associate
 * multiple values to each key. Which is exactly what we need to
 * represent an directed graph – the values associated with each key
 * will be the successors of the vertex given by the key. */

using graph = std::multimap< int, int >;

/* The algorithm consists of a single function, ‹distance_to_odd›,
 * which takes the graph ‹g›, as a constant reference, and the
 * vertex ‹initial›, as arguments. It then returns the sought
 * distance, or if no matching vertex is found, -1. */

int distance_to_odd( const graph &g, int initial )
{
    /* We start by declaring the «visited set», which prevents the
     * algorithm from getting stuck in an infinite loop, should it
     * encounter a cycle in the input graph (and also helps to keep
     * the time complexity under control). */

    std::set< int > visited;

    /* The next piece of the algorithm is the «exploration queue»:
     * we will put two pieces of information into the queue: first,
     * the vertex to be explored, second, its BFS distance from
     * ‹initial›. */

    std::queue< std::pair< int, int > > queue;

    /* To kickstart the exploration, we place the initial vertex,
     * along with distance 0, into the queue: */

    queue.emplace( initial, 0 );

    /* Follows the standard BFS loop: */

    while ( !queue.empty() )
    {
        auto [ vertex, distance ] = queue.front();
        queue.pop();

        /* To iterate all the successors of a vertex in an
         * ‹std::multimap›, we will use its ‹equal_range› method,
         * which will return a pair of «iterators» – generalized
         * pointers, which support a kind of ‘pointer arithmetic’.
         * The important part is that an iterator can be incremented
         * using the ‹++› operator to get the next element in a
         * sequence, and dereferenced using the unary ‹*› operator
         * to get the pointed-to element. The result of
         * ‹equal_range› is a pair of iterators:
         *
         *  • begin, which points at the first matching key-value
         *    pair in the multimap,
         *  • end, which points «one past» the last matching
         *    element; clearly, if ‹begin == end›, the sequence is
         *    empty.
         *
         *  Incrementing ‹begin› will eventually cause it to become
         *  equal to ‹end›, at which point we have reached the end
         *  of the sequence. Let's try: */

        auto [ begin, end ] = g.equal_range( vertex );

        for ( ; begin != end; ++ begin )
        {
            /* In the body loop, ‹begin› points, in turn, at each
             * matching key-value pair in the graph. To get the
             * corresponding value (which is what we care about), we
             * extract the second element: */

            auto [ _, next ] = *begin;

            if ( visited.contains( next ) )
                continue; /* skip already-visited vertices */

            /* First, let us check whether we have found the vertex
             * we were looking for: */

            if ( next % 2 == 1 )
                return distance + 1;

            /* Otherwise we mark the vertex as visited and put it
             * into the queue, continuing the search. */

            visited.insert( next );
            queue.emplace( next, distance + 1 );
        }
    }

    /* We have exhausted the queue, and hence all the vertices
     * reachable from ‹initial›, without finding an odd-valued
     * one. Indicate failure to the caller. */

    return -1;
}

int main() /* demo */
{
    graph g{ { 1, 2 }, { 1, 6 }, { 2, 4 }, { 2, 5 }, { 6, 4 } },
          h{ { 8, 2 }, { 8, 6 }, { 2, 4 }, { 2, 5 }, { 5, 8 } },
          i{ { 2, 4 }, { 4, 2 } };

    assert( distance_to_odd( g, 1 ) ==  2 );
    assert( distance_to_odd( g, 2 ) ==  1 );
    assert( distance_to_odd( g, 6 ) == -1 );

    assert( distance_to_odd( h, 8 ) ==  2 );
    assert( distance_to_odd( h, 5 ) ==  3 );
    assert( distance_to_odd( i, 2 ) == -1 );
}

/* ¹ Strictly speaking, ‹std::queue› is not a container, but rather
 *   a container «adaptor». Internally, unless specified otherwise,
 *   an ‹std::queue› uses another container, ‹std::deque› to store
 *   the data and implement the operations. It would also be
 *   possible, though less convenient, to use ‹std::deque›
 *   directly. */
