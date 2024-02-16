#include <cassert>

/* Naprogramujte typ ‹tictactoe›, který bude reprezentovat stav této
 * jednoduché hry (piškvorky na ploše 3×3). Stav hry má tyto složky:
 *
 *  • který hráč je na tahu,
 *  • který hráč zabral která políčka.
 *
 * V nově vytvořené hře je plocha prázdná a na tahu je hrač
 * s křížky. Metody:
 *
 *  • ‹play( x, y )› umístí symbol aktivního hráče na souřadnice
 *    ‹x›, ‹y› (platnost souřadnic i tahu je vstupní podmínkou, roh
 *    má souřadnice ⟦0, 0⟧),
 *  • ‹read( x, y )› vrátí hodnotu zadaného pole:
 *    ◦ -1 je křížek,
 *    ◦ 0 je prázdné pole, konečně 
 *    ◦ 1 je kolečko,
 *  • ‹winner()› vrátí podobně -1/0/1 podle toho, který hráč vyhrál
 *    (0 značí, že hra buď ještě neskončila, nebo skončila remízou).
 */

struct tictactoe;

int main()
{
    tictactoe t;
    const tictactoe &const_t = t;

    assert( const_t.read( 0, 0 ) == 0 );
    assert( const_t.read( 2, 1 ) == 0 );
    assert( const_t.winner() == 0 );

    t.play( 0, 0 );
    t.play( 1, 0 );
    assert( const_t.read( 0, 0 ) == -1 );
    assert( const_t.read( 1, 0 ) == +1 );
    assert( const_t.winner() == 0 );
    t.play( 0, 1 );
    t.play( 1, 1 );
    t.play( 0, 2 );
    assert( const_t.winner() == -1 );

    return 0;
}
