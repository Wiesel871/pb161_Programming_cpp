#include <cstdint>
#include <cassert>

/* Najděte a vraťte číslo, které vznikne zapsáním (nezáporných)
 * celých čísel ‹a›, ‹b› v binárním zápisu za sebe (zápis čísla ‹a›
 * bude vlevo, zápis čísla ‹b› vpravo a bude doplněn levostrannými
 * nulami na délku ‹b_bits› bitů). Je-li zápis čísla ‹b› delší než
 * ‹b_bits›, výsledek není definován.
 *
 * Příklad: ‹concat( 1, 1, 2 )› vrátí hodnotu ‹0b101 = 5›. */

std::uint64_t concat( std::uint64_t a,
                      std::uint64_t b, int b_bits );

int main()
{
    assert( concat( 1, 1, 2 ) == 5 );
    assert( concat( 1, 1, 1 ) == 3 );
    assert( concat( 2, 1, 1 ) == 5 );
    assert( concat( 3, 1, 1 ) == 7 );
    assert( concat( 3, 1, 2 ) == 13 );
    assert( concat( 0x10, 0x10, 16 ) == 0x100010 );
    assert( concat( 0x11, 0x10, 16 ) == 0x110010 );

    return 0;
}
