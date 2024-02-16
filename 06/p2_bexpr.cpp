#include <cassert>
#include <map>
#include <stdexcept>

/* Boolean expressions with variables, represented as binary trees.
 * Internal nodes carry a logical operation on the values obtained
 * from children while leaf nodes carry variable references. */

/* To evaluate an expression, we will need to supply values for each
 * of the variables that appears in the expression. We will identify
 * variables using integers, and the assignment of values will be
 * done through the type ‹input› defined below. It is undefined
 * behaviour if a variable appears in an expression but is not
 * present in the provided ‹input› value. */

using input = std::map< int, bool >;

/* Like earlier in ‹expr.cpp›, the base class will be called ‹node›,
 * but this time will only define a single method: ‹eval›, which
 * accepts a single ‹input› argument (as a ‹const› reference). */

class node; /* ref: 6 lines */

/* Internal nodes are all of the same type, and their constructor
 * takes an unsigned integer, ‹table›, and two ‹node› references.
 * Assuming bit zero is the lowest-order bit, the node operates as
 * follows:
 *
 *  • ‹false› ‹false› → bit 0 of ‹table›
 *  • ‹false› ‹true›  → bit 1 of ‹table›
 *  • ‹true›  ‹false› → bit 2 of ‹table›
 *  • ‹true›  ‹true›  → bit 3 of ‹table› */

class operation; /* ref: 16 lines */

/* The leaf nodes carry a single integer (passed in through the
 * constructor) -- the identifier of the variable they represent. */

class variable; /* ref: 7 lines */

int main()
{
    input i_0{ { 1, false }, { 2, false } };
    input i_1{ { 1, false }, { 2, true  } };
    input i_2{ { 1, true  }, { 2, false } };
    input i_3{ { 1, true  }, { 2, true  } };

    variable var_1( 1 ), var_2( 2 );

    assert( var_1.eval( i_0 ) == false );
    assert( var_1.eval( i_2 ) == true );

    assert( var_2.eval( i_0 ) == false );
    assert( var_2.eval( i_1 ) == true  );
    assert( var_2.eval( i_2 ) == false );
    assert( var_2.eval( i_3 ) == true  );

    /* basic operations */

    unsigned op_xor = 0b0110, op_nor = 0b0001;

    operation zero( op_xor, var_1, var_1 ),
              one( op_nor, zero, zero );

    assert( !zero.eval( i_0 ) );
    assert( !zero.eval( i_1 ) );
    assert( !zero.eval( i_2 ) );
    assert( !zero.eval( i_3 ) );

    assert( one.eval( i_0 ) );
    assert( one.eval( i_1 ) );
    assert( one.eval( i_2 ) );
    assert( one.eval( i_3 ) );

    operation x1( op_xor, var_1, var_2 );
    assert( !x1.eval( i_0 ) );
    assert(  x1.eval( i_1 ) );
    assert(  x1.eval( i_2 ) );
    assert( !x1.eval( i_3 ) );

    operation x2( op_xor, x1, var_1 );
    assert( !x2.eval( i_0 ) );
    assert(  x2.eval( i_1 ) );
    assert( !x2.eval( i_2 ) );
    assert(  x2.eval( i_3 ) );

    return 0;
}
