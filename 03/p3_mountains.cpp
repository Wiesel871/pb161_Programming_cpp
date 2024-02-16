#include <cassert>
#include <cstdint>
#include <vector>

/* Vaším úkolem je vytvořit typ ‹mountain_range›, který bude
 * reprezentovat rekurzivní pohoří. Rekurzivní pohoří má tento tvar:
 *
 *  1. levý svah (může být prázdný), který může na každém kroku
 *     libovolně stoupat,
 *  2. libovolný počet (i nula) vnitřních pohoří stejného typu
 *     (první z nich začíná ve výšce, na které skončil levý svah),
 *  3. pravý svah, který je zrcadlovým obrazem toho levého.
 *
 * Například (hlavní pohoří má prázdný svah; závorky naznačují
 * začátky a konce jednotlivých vnitřních pohoří):
 *
 *          ┌───┐                   ┌───┐
 *          │   │ ┌───┐     ┌───┐ ┌─┘   └─┐
 *        ┌─┘   └─┘   │   ┌─┘   └─┘       │
 *      ┌─┘           └───┘               └─┐
 *     ╶┘                                   └╴
 *       1 2 4 4 2 3 3 1 1 2 3 3 2 3 4 4 3 1
 *       ( (     )     ) ( (     ) (     ) )
 *
 * Je-li ‹outer› hodnota typu ‹mountain_range›, nechť:
 *
 *  1. ‹outer.get( i )› vrátí výšku ‹i›-tého pole pohoří ‹outer›, a
 *  2. ‹outer.set_slope( slope )› pro zadaný vektor čísel ‹slope›
 *     nastaví «oba» svahy tak, aby ten «levý» odpovídal výškám
 *     v ‹slope›,
 *  2. ‹outer.insert( inner )› vloží nové vnitřní pohoří zadané
 *     hodnotou ‹inner› typu ‹mountain_range›, a to těsně před pravý
 *     svah.
 *
 * Dobře si rozmyslete vhodnou reprezentaci. Požadujeme:
 *
 *  • metoda ‹get› musí mít konstantní složitost,
 *  • metoda ‹set_slope› může být vůči argumentu lineární, ale nesmí
 *    záviset na délce vnitřních pohoří,
 *  • metoda ‹insert› může být vůči vkládanému pohoří (‹inner›)
 *    lineární, vůči tomu vnějšímu (‹outer›) ale musí být
 *    amortizovaně konstantní.
 *
 * Nově vytvořená hodnota typu ‹mountain_range› reprezentuje prázdné
 * pohoří (prázdný svah a žádná vnitřní pohoří). */

struct mountain_range;

int main()
{
    mountain_range a, b;
    const mountain_range &a_const = a, &b_const = b;

    a.set_slope( { 1, 2 } );

    assert( a.get( 0 ) == 1 );
    assert( a.get( 1 ) == 2 );
    assert( a.get( 2 ) == 2 );
    assert( a.get( 3 ) == 1 );

    b = a;
    a.insert( b );

    assert( a.get( 0 ) == 1 );
    assert( a.get( 1 ) == 2 );
    assert( a.get( 2 ) == 3 );
    assert( a.get( 3 ) == 4 );
    assert( a.get( 4 ) == 4 );
    assert( a.get( 5 ) == 3 );
    assert( a.get( 6 ) == 2 );
    assert( a.get( 7 ) == 1 );

    a.insert( b_const );

    assert( a_const.get( 0 ) == 1 );
    assert( a_const.get( 1 ) == 2 );
    assert( a_const.get( 2 ) == 3 );
    assert( a_const.get( 3 ) == 4 );
    assert( a_const.get( 4 ) == 4 );
    assert( a_const.get( 5 ) == 3 );
    assert( a_const.get( 6 ) == 3 );
    assert( a_const.get( 7 ) == 4 );
    assert( a_const.get( 8 ) == 4 );
    assert( a_const.get( 9 ) == 3 );
    assert( a_const.get( 10 ) == 2 );
    assert( a_const.get( 11 ) == 1 );

    return 0;
}
