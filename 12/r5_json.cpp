#include <map>
#include <string>
#include <cassert>

/* You are given a single-level string → string dictionary. Turn it
 * into a single string, using JSON as the format. Take care to
 * escape special characters – at least double quote and the escape
 * character (backslash) itself.
 *
 * In JSON, key order is not important – emit them in iteration
 * (alphabetic) order. Put a single space after each ‘element’:
 * after the opening brace, after colons and after commas, except if
 * the input is empty, in which case the output should be just ‹{}›.
 */

using str_dict = std::map< std::string, std::string >;

std::string to_json( const str_dict &dict );

int main()
{
    str_dict in_0 = { { "foo", "wibble" } };
    std::string out_0 = "{ \"foo\": \"wibble\" }";

    str_dict in_1 = { { "foo", "wibble" }, { "bar", "quux" } };
    std::string out_1 = "{ \"bar\": \"quux\", \"foo\": \"wibble\" }";

    str_dict in_2 = { { "fo\"o", "wi\\bble" }, { "bar", "quux" } };
    std::string out_2 = "{ \"bar\": \"quux\", \"fo\\\"o\": \"wi\\\\bble\" }";

    assert( to_json( in_0 ) == out_0 );
    assert( to_json( in_1 ) == out_1 );
    assert( to_json( in_2 ) == out_2 );
    assert( to_json( str_dict() ) == "{}" );

    return 0;
}
