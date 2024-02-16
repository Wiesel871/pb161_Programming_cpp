#include <stack>

/* In this exercise, we will write a pretty-printer for simple
 * arithmetic expressions, with 3 operation types: addition,
 * multiplication and equality, written as ‹+›, ‹*› and ‹=›
 * respectively. The goal is to print the expression with as few
 * parentheses as possible.
 *
 * Assume full associativity for all operations. The precedence
 * order is the usual one: multiplication binds most tightly, while
 * equality most loosely.
 *
 * The formatting is done by calling a ‹print› method on the root of
 * the expression to be printed. */

class node;
class addition;
class multiplication;
class equality;
class constant;

using node_ptr = std::unique_ptr< node >;

node_ptr read( std::string_view expr );

int main()
{
    auto eq = []( auto rpn, auto pretty )
    {
        return read( rpn )->print() == pretty;
    };

    assert( eq( "3",             "3" ) );
    assert( eq( "3 1 +",         "3 + 1" ) );
    assert( eq( "7 3 1 + +",     "7 + 3 + 1" ) );
    assert( eq( "7 3 1 * +",     "7 + 3 * 1" ) );
    assert( eq( "7 3 1 + *",     "7 * ( 3 + 1 )" ) );
    assert( eq( "7 3 1 + =",     "7 = 3 + 1" ) );
    assert( eq( "7 3 1 + 2 + *", "7 * ( 3 + 1 + 2 )" ) );

    assert( eq( "7 3 + 1 * 2 + 4 *",
                "( ( 7 + 3 ) * 1 + 2 ) * 4" ) );

    return 0;
}

template< typename T >
auto make_bin( std::stack< node_ptr > &stack )
{
    assert( stack.size() >= 2 );
    auto right = std::move( stack.top() );
    stack.pop();
    auto left = std::move( stack.top() );
    stack.pop();
    stack.emplace( new T( std::move( left ), std::move( right ) ) );
}

node_ptr read( std::string_view expr )
{
    std::stack< node_ptr > stack;
    std::string number;

    while ( !expr.empty() )
    {
        number.clear();

        while ( !expr.empty() && std::isspace( expr[ 0 ] ) )
            expr.remove_prefix( 1 );

        while ( !expr.empty() && std::isdigit( expr[ 0 ] ) )
        {
            number += expr[ 0 ];
            expr.remove_prefix( 1 );
        }

        if ( !number.empty() )
        {
            stack.emplace( new constant( std::stoi( number ) ) );
            continue;
        }

        if ( !expr.empty() )
        {
            switch ( expr[ 0 ] )
            {
                case '+': make_bin< addition >( stack ); break;
                case '*': make_bin< multiplication >( stack ); break;
                case '=': make_bin< equality >( stack ); break;
                default: assert( false );
            }
            expr.remove_prefix( 1 );
        }
    }

    assert( stack.size() == 1 );
    return std::move( stack.top() );
}
