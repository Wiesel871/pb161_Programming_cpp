#include <variant>
#include <memory>
#include <cassert>

/* Máte zadané následující typy, které reprezentují jednoduchý
 * aritmetický výraz. */

struct constant;
struct add;
struct subtract;
struct multiply;
struct divide;

using expr = std::variant< constant, add, subtract, multiply, divide >;
using expr_ptr = std::unique_ptr< expr >;

struct constant
{
    constant( int v ) : value( v ) {}
    int value = 0;
};

struct binary
{
    expr_ptr left, right;
    binary( expr a, expr b );
};

struct add      : binary { using binary::binary; };
struct subtract : binary { using binary::binary; };
struct multiply : binary { using binary::binary; };
struct divide   : binary { using binary::binary; };

binary::binary( expr a, expr b )
    : left{ std::make_unique< expr >( std::move( a ) ) },
      right{ std::make_unique< expr >( std::move( b ) ) }
{}

/* Vaším úkolem je naprogramovat funkci ‹eval›, která takto zadaný
 * výraz vyhodnotí na celé číslo. */

int main()
{
    expr e1 = add( multiply( 1, 2 ), 1 );
    expr e2 = add( subtract( 3, multiply( 1, 2 ) ), 1 );

    assert( eval( e1 ) == 3 );
    assert( eval( e2 ) == 2 );

    return 0;
}
