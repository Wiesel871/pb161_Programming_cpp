#include <cassert>
#include <utility>

/* Implement a coffee machine which gives out a token when the order
 * is placed and takes the token back when it is done… at most one
 * order can be in progress. */

/* Throw this when the machine is already busy making coffee. */

class busy {};

/* And this when trying to use a default-constructed or already-used
 * token. */

class invalid {};

/* Fill in the two classes. Besides constructors and assignment
 * operators, add methods ‹make› and ‹fetch› to ‹machine›, to create
 * and redeem tokens respectively. */

class machine;
class token;

int main()
{
    machine m;

    token t_1 = m.make();
    token t_2;

    using inv_t = const invalid &;

    try { m.fetch( t_2 ); assert( false ); } catch ( inv_t ) {};
    try { m.make(); assert( false ); } catch ( const busy & ) {};

    token t_3 = std::move( t_1 );
    try { m.fetch( t_1 ); assert( false ); } catch ( inv_t ) {};

    t_2 = std::move( t_3 );
    try { m.fetch( t_3 ); assert( false ); } catch ( inv_t ) {};

    m.fetch( t_2 );

    return 0;

    // clang-tidy: -clang-analyzer-deadcode.DeadStores
}
