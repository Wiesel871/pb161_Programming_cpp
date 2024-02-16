#include <string>
#include <bitset>
#include <vector>
#include <fcntl.h>
#include <system_error>
#include <thread>
#include <cassert>

/* It is possible to sub-class standard exception classes. For most
 * uses, ‹std::runtime_error› is the most appropriate base class. */

class custom_exception : public std::runtime_error
{
public:
    custom_exception() : std::runtime_error( "custom" ) {}
};

/* This demo simply demonstrates some of the standard exception
 * types (i.e. those that are part of the standard library, and
 * which are thrown by standard functions or methods; as long as
 * those methods or functions are not too arcane). */

int main() /* demo */
{
    try
    {
        throw custom_exception();
        assert( false );
    }
    /* As per standard rules, it's possible to catch exceptions of
     * derived classes (of course including user-defined types) via
     * a ‹catch› clause which accepts a reference to a superclass. */
    catch ( const std::exception & ) {}

    try
    {
        std::vector x{ 1, 2 };
        /* Attempting out-of-bounds access through ‹at› gives
         * ‹std::out_of_range› */
        x.at( 7 );
        assert( false );
    }
    catch ( const std::out_of_range & ) {}

    try
    {
        /* If the string passed to ‹stoi› is not a number, we get
         * back an exception of type ‹std::invalid_argument›. */
        std::stoi( "foo" );
        assert( false );
    }
    catch ( const std::invalid_argument & ) {}

    try
    {
        /* If an integer is too big to fit the result type, ‹stoi›
         * throws ‹std::out_of_range›. */
        std::stoi( "123456123456123456" );
        assert( false );
    }
    catch ( const std::out_of_range & ) {}

    try
    {
        /* System-interfacing functions may throw
         * ‹std::system_error›. Here, for instance, trying to detach
         * a thread which was not started. */
        std::thread().detach();
        assert( false );
    }
    catch ( const std::system_error & ) {}

    try
    {
        /* Throwing a ‹system_error› is the appropriate reaction
         * when dealing with a failure of a POSIX function which
         * sets ‹errno›. */

        int fd = ::open( "/does/not/exist", O_RDONLY );
        if ( fd < 0 )
            throw std::system_error( errno, std::system_category(),
                                     "opening /does/not/exist" );
        assert( false );
    }
    catch ( const std::system_error & ) {}

    try
    {
        /* Passing a size that is more than ‹max_size()› when
         * constructing or resizing an ‹std::string› or an
         * ‹std::vector› gives us back an ‹std::length_error›. Note
         * that the -1 turns into a really big number in this
         * context. */

        std::string x( -1, 'x' );
        assert( false );
    }
    catch ( const std::length_error & ) {}

    try
    {
        std::bitset< 128 > x;
        x[ 100 ] = true;

        /* Trying to convert an ‹std::bitset› to an integer type may
         * throw ‹std::overflow_error›, if there are bits set that
         * do not fit into the target integer type. */
        x.to_ulong();
        assert( false );
    }
    catch ( const std::overflow_error & ) {}
}
