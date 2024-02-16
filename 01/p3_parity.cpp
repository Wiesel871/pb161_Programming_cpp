#include <cassert>

/* Funkce ‹parity› zjistí, je-li počet jedniček na vstupu sudý
 * (výsledkem je ‹false›) nebo lichý (výsledkem je ‹true›).
 *
 * Jedničky počítáme v binárním zápisu vstupního bezznaménkového
 * čísla ‹word›. Je-li navíc ‹chksum› na začátku ‹true›, počítá se
 * jako další jednička.  Celkový výsledek jednak uložte do parametru
 * ‹chksum›, jednak ho vraťte jako návratovou hodnotu. */

bool parity( auto word, bool &chksum );

int main()
{
    bool chksum = false;

    assert( parity( 1, chksum ) && chksum );
    assert( !parity( 1, chksum ) && !chksum );
    assert( parity( 1, chksum ) && chksum );
    assert( parity( 3, chksum ) && chksum );
    assert( !parity( 4, chksum ) && !chksum );
    assert( !parity( 5, chksum ) && !chksum );
    assert( !parity( 0xff, chksum ) && !chksum );
    assert( parity( 0x1ffffffffu, chksum ) && chksum );

    return 0;
}
