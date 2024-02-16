#include <memory>
#include <string>
#include <cassert>

/* The goal of this exercise is to implement a printer for JSON,
 * invoked as a ‹print› method available on each ‹node›. It should
 * take no arguments and return an instance of ‹std::string›. For
 * simplicity, the only scalar values you need to implement are
 * integers. Then there are 2 composite data types:
 *
 *  • arrays, which represent a sequence of arbitrary values,
 *  • objects, which map strings to arbitrary values.
 *
 * Both composite types are «heterogeneous» (the items can be of
 * different types). They are formatted as follows:
 *
 *  • array: ‹[ 1, [ 2, 3 ], 4 ]›,
 *  • object: ‹{ "key₁": 7, "key₂": [ 1, 2 ] }›.
 *
 * To further simplify matters, we will not deal with line breaks or
 * indentation – format everything as a single line. */

class node;
using node_ptr = std::unique_ptr< node >;
using node_ref = const node &;

/* The ‹number› class is to be constructed from an ‹int›, has no
 * children, and needs no methods besides ‹print›. */

class number;

/* The ‹object› and ‹array› classes represent composite JSON data.
 * They should be both default-constructible, resulting in an empty
 * collection. Both should have an ‹append› method: for ‹object›, it
 * takes an ‹std::string› (the key) and a ‹node_ptr›, while for
 * ‹array›, only the latter. In both cases, print items in the order
 * in which they were appended. Duplicated keys are ignored (i.e.
 * first occurrence wins). */

class object;
class array;

int main()
{
    auto mk_const = []( int v )
    {
        return std::make_unique< number >( v );
    };

    auto obj = std::make_unique< object >();
    assert( obj->print() == "{}" );

    auto arr = std::make_unique< array >();
    assert( arr->print() == "[]" );

    obj->append( "key₁", mk_const( 7 ) );
    assert( obj->print() == "{ \"key₁\": 7 }" );

    arr->append( mk_const( 1 ) );
    assert( arr->print() == "[ 1 ]" );
    arr->append( mk_const( 2 ) );
    assert( arr->print() == "[ 1, 2 ]" );

    obj->append( "key₂", std::move( arr ) );
    assert( obj->print() == "{ \"key₁\": 7, \"key₂\": [ 1, 2 ] }" );
    obj->append( "key₃", std::make_unique< object >() );
    assert( obj->print() ==
            "{ \"key₁\": 7, \"key₂\": [ 1, 2 ], \"key₃\": {} }" );

    return 0;
}
