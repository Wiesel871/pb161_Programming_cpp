#include <cassert>

/* Cílem cvičení je naprogramovat typ, který bude reprezentovat
 * polynomy s celočíselnými koeficienty, s operacemi sčítání
 * (jednoduché) a násobení (méně jednoduché).  Polynom je výraz
 * tvaru ⟦7x⁴ + 0x³ + 0x² + 3x + x⁰⟧ – tzn. součet nezáporných
 * celočíseslných mocnin proměnné ⟦x⟧, kde u každé mocniny stojí
 * pevný (konstantní) koeficient.
 *
 * Součet polynomů má u každé mocniny součet koeficientů příslušné
 * mocniny sčítanců. Součin je složitější, protože:
 *
 *  • každý monom (sčítanec) prvního polynomu musí být vynásoben
 *    každým monomem druhého polynomu,
 *  • některé z těchto součinů vedou na stejnou mocninu ⟦x⟧ a tedy
 *    jejich koeficienty musí být sečteny.
 *
 * Pro každý polynom existuje nějaké ⟦n⟧ takové, že všechny mocniny
 * větší než ⟦n⟧ mají nulový koeficient. Tento fakt nám umožní
 * polynomy snadno reprezentovat. */

/* Implicitně sestrojená hodnota typu ‹poly› nechť má všechny
 * koeficienty nulové. Krom sčítání a násobení (formou operátorů)
 * implementujte také rovnost a metodu ‹set›, která má dva
 * parametry: mocninu ⟦x⟧ a koeficient, obojí celá čísla. */

struct poly;

int main()
{
    auto mkpoly = []( auto... coeff )
    {
        poly p;
        int i = sizeof...( coeff );
        for ( int c : { coeff... } )
            p.set( --i, c );
        return p;
    };

    const poly a       = mkpoly( 1, 0, 1 );       // x² + 1
    const poly b       = mkpoly( 2, 0, 1 );       // 2x² + 1
    const poly c       = mkpoly( 1, 1 );          // x + 1
    const poly d       = mkpoly( 1, 0 );          // x
    const poly sum_ab  = mkpoly( 3, 0, 2 );       // 3x² + 2
    const poly sum_ac  = mkpoly( 1, 1, 2 );       // x² + x + 2
    const poly sum_ad  = mkpoly( 1, 1, 1 );       // x² + x + 1
    const poly prod_ab = mkpoly( 2, 0, 3, 0, 1 ); // 2x⁴ + 3x² + 1
    const poly prod_ac = mkpoly( 1, 1, 1, 1 );    // x³ + x² + x + 1
    const poly prod_cd = mkpoly( 1, 1, 0 );       // x² + x
    const poly prod_bd = mkpoly( 2, 0, 1, 0 );    // 2x³ + x

    assert( a + b == sum_ab );
    assert( b + a == sum_ab );
    assert( a + c == sum_ac );
    assert( c + a == sum_ac );
    assert( a + d == sum_ad );
    assert( d + a == sum_ad );

    assert( a * b == prod_ab );
    assert( b * a == prod_ab );
    assert( a * c == prod_ac );
    assert( c * a == prod_ac );
    assert( c * d == prod_cd );
    assert( d * c == prod_cd );
    assert( b * d == prod_bd );
    assert( d * b == prod_bd );

    return 0;
}
