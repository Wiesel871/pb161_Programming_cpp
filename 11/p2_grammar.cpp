#include <cassert>
#include <string>
#include <vector>

/* Regulární gramatika má pravidla tvaru ⟦A → xB⟧ nebo ⟦A → x⟧ kde
 * ⟦A⟧, ⟦B⟧ jsou neterminály a ⟦x⟧ je terminál. */

/* Navrhněte typ ‹grammar›, kterého hodnoty lze implicitně sestrojit
 * a který má tyto 2 metody:
 *
 *  • ‹add_rule›, které lze předat 2 nebo 3 parametry:
 *    a. jeden znak, který reprezentuje levou stranu (velké písmeno
 *       anglické abecedy) a
 *    b. jeden terminál (malé písmeno), nebo jeden terminál a jeden
 *       neterminál,
 *  • ‹generate›, která přijme 2 parametry: startovní neterminál a
 *    kladné celé číslo, které reprezentuje maximální délku slova;
 *    výsledkem bude seznam (‹std::vector›) všech řetězců (slov),
 *    které lze takto vygenerovat, a to v lexikografickém pořadí. */

class grammar;

int main()
{
    auto w = []( auto... words ) /* helper function */
    {
        return std::vector< std::string >{ words... };
    };

    grammar g;
    const grammar &cg = g;

    assert( cg.generate( 'A', 10 ) == w() );
    g.add_rule( 'A', 'a' );
    assert( cg.generate( 'A', 10 ) == w( "a" ) );

    g.add_rule( 'A', 'b', 'A' );
    assert( cg.generate( 'A', 3 ) == w( "a", "ba", "bba" ) );

    g.add_rule( 'A', 'x', 'X' );
    g.add_rule( 'X', 'x' );
    assert( cg.generate( 'A', 3 ) == w( "a", "ba", "bba", "bxx", "xx" ) );
    g.add_rule( 'X', 'x', 'X' );
    assert( cg.generate( 'X', 3 ) == w( "x", "xx", "xxx" ) );

    return 0;
}
