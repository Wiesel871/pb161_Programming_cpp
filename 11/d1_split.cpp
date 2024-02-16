#include <string_view>
#include <cassert>

/* Let's implement a pure function ‹split›, which given a string
 * view ‹s› and a delimiter ‹delim›, produces a pair of string_views
 * ‹a› and ‹b› such that:
 *
 *  • ‹delim› is not in ‹a›,
 *  • and either
 *    ◦ ‹s == a + delim + b› if ‹delim› was present in ‹s›,
 *    ◦ ‹s == a› and ‹b› is empty otherwise */

using split_view = std::pair< std::string_view, std::string_view >;

split_view split( std::string_view s, char delim )
{
    size_t idx = s.find( delim );
    if ( idx == s.npos )
        return { s, "" };
    else
        return { s.substr( 0, idx ), s.substr( idx + 1, s.npos ) };
}

int main() /* demo */
{
    auto [ a, b ] = split( "hello world", ' ' );
    assert( a == "hello" );
    assert( b == "world" );

    auto [ c, d ] = split( "hello world", '!' );
    assert( c == "hello world" );
    assert( d == "" );

    return 0;
}
