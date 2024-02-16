#include <cassert>
#include <vector>

/* V této ukázce si předvedeme práci s tzv. funkcemi vyššího řádu
 * (higher order) – tedy funkcemi, kterých parametrem je opět
 * funkce. */

auto iterate( auto f, auto x, int count )
{
    /* We want to build a vector of values, starting with ‹x›, then
     * ‹f(x)›, ‹f(f(x))›, and so on. Immediately, we face a problem:
     * what should be the type of the vector? We need to specify the
     * type parameter to declare the variable, and this time we
     * won't be able to weasel out by just saying ‹auto›, since the
     * compiler can't tell the type without an unambiguously typed
     * initializer. We have two options here:
     *
     *  1. we could omit the type parameter of ‹std::vector› and let
     *     the compiler deduce only that – this would be written
     *     ‹std::vector out{ x }› – by putting ‹x› into the vector
     *     right from the start, the compiler can deduce that the
     *     element type should be the same as the type of ‹x›,
     *     whatever that is,
     *  2. we can use ‹decltype› to obtain the type of ‹x› and use
     *     that to specify the required type parameter for ‹out›,
     *     i.e.: */

    std::vector< decltype( x ) > out;
    out.push_back( x );

    /* We build the return vector by repeatedly calling ‹f› on the
     * previous value, until we hit ‹count› items. */

    for ( int i = 1; i < count; ++ i )
        out.push_back( f( out.back() ) );

    return out;
};

int main() /* demo */
{
    /* Besides the missing name and the empty square brackets, the
     * signature of the lambda is similar to a standard function.
     * However, on closer inspection, another thing is missing: the
     * return type. This might be specified using ‹-> type› after the
     * argument list, but if it is not, the compiler will, again, deduce
     * the type for us. The return type is commonly omitted. */

    auto f = []( int x )
    {
        /* We return a value just like in a regular function. Please
         * also note the semicolon after the closing brace: definition
         * of a lambda is an «expression», and the variable declaration
         * as a whole needs to be delimited by a semicolon, just like in
         * ‹int x = 7;›. */

        return x * x;
    };

    auto g = []( int x ) { return x + 1; };

    auto v = iterate( f, 2, 4 );

    std::vector< int > expect{ 2, 4, 16, 256 };
    assert( v == expect );

    std::vector< int >
        iota = iterate( g, 1, 4 ),
        iota_expect{ 1, 2, 3, 4 };

    assert( iota == iota_expect );
}
