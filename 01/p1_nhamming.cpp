#include <cassert>


/* Nezáporná čísla ‹a›, ‹b› zapíšeme v poziční soustavě o základu
 * ‹base›. Spočtěte hammingovu vzdálenost těchto dvou zápisů (přitom
 * zápis kratšího čísla podle potřeby doplníme levostrannými
 * nulami). */

int hamming( int a, int b, int base ) {
    if (a == b)
        return 0;
    int res = 0;
    for (; a > 0 && b > 0; a /= base, b /= base)
        res += (a % base != b % base);
    for (; a > 0; a /= base)
        res += (a % base != 0);
    for (; b > 0; b /= base)
        res += (b % base != 0);
    return res;
}

int main() {
    assert( hamming( 1, 3, 2 ) == 1 );
    assert( hamming( 1, 3, 3 ) == 2 );
    assert( hamming( 3, 3, 3 ) == 0 );
    assert( hamming( 0, 1, 3 ) == 1 );
    assert( hamming( 0, 2, 3 ) == 1 );

    return 0;
}
