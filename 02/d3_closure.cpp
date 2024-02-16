#include <set>
#include <utility>
#include <cassert>

/* In this demo, we will check closure properties of relations:
 * reflexivity, transitivity and symmetry. A relation is a set of
 * pairs, and hence we will represent them as ‹std::set› of
 * ‹std::pair› instances. We will work with relations on integers.
 * Recall that ‹std::set› has an efficient membership test: we will
 * be using that a lot in this program. */

using relation = std::set< std::pair< int, int > >;

/* The first predicate checks reflexivity: for any ⟦x⟧ which appears
 * in the relation, the pair ⟦(x, x)⟧ must be present. Besides
 * membership testing, we will use structured bindings and range
 * ‹for› loops. Notice that a braced list of values is implicitly
 * converted to the correct type (‹std::pair< int, int >›). */

bool is_reflexive( const relation &r )
{

    /* Structured bindings are written using ‹auto›, followed by
     * square brackets with a list of names to bind to individual
     * components of the right-hand side. In this case, the
     * right-hand side is the «loop variable» – i.e. each of the
     * elements of ‹r› in turn. */

    for ( auto [ x, y ] : r )
    {
        if ( !r.contains( { x, x } ) )
            return false;
        if ( !r.contains( { y, y } ) )
            return false;
    }

    /* We have checked all the elements of ‹r› and did not find any
     * which would violate the required property. Return ‹true›. */

    return true;
}

/* Another, even simpler, check is for symmetry. A relation is
 * symmetric if for any pair ⟦(x, y)⟧ it also contains the opposite,
 * ⟦(y, x)⟧. */

bool is_symmetric( const relation &r )
{
    for ( auto [ x, y ] : r )
        if ( !r.contains( { y, x } ) )
            return false;
    return true;
}

/* Finally, a slightly more involved example: transitivity. A
 * relation is transitive if ⟦∀x, y, z. (x, y) ∈ r ∧ (y, z) ∈ r →
 * (x, z) ∈ r⟧. */

bool is_transitive( const relation &r )
{
    for ( auto [ x, y ] : r )
        for ( auto [ y_prime, z ] : r )
            if ( y == y_prime && !r.contains( { x, z } ) )
                return false;
    return true;
}

int main() /* demo */
{
    relation r_1{ { 1, 1 }, { 1, 2 } };
    assert( !is_reflexive( r_1 ) );
    assert( !is_symmetric( r_1 ) );
    assert(  is_transitive( r_1 ) );

    relation r_2{ { 1, 1 }, { 1, 2 }, { 2, 2 } };
    assert(  is_reflexive( r_2 ) );
    assert( !is_symmetric( r_2 ) );
    assert(  is_transitive( r_2 ) );

    relation r_3{ { 2, 1 }, { 1, 2 }, { 2, 2 } };
    assert( !is_reflexive( r_3 ) );
    assert(  is_symmetric( r_3 ) );
    assert( !is_transitive( r_3 ) );
}
