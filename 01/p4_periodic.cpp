#include <cstdint>
#include <cassert>
#include <cstdio>
#include <iostream>

/* Najděte nejmenší nezáporné číslo ⟦n⟧ takové, že 64-bitový zápis
 * čísla ‹word› lze získat zřetězením nějakého počtu binárně
 * zapsaných kopií ⟦n⟧. Protože potřebný zápis ⟦n⟧ může obsahovat
 * levostranné nuly, do výstupního parametru ‹length› uložte jeho
 * délku v bitech. Je-li možné ‹word› sestavit z různě dlouhých
 * zápisů nějakého ⟦n⟧, vyberte ten nejkratší možný.
 *
 * Příklad: pro ‹word = 0x100000001› bude hledané ⟦n = 1⟧, protože
 * ‹word› lze zapsat jako dvě kopie čísla 1 zapsaného do 32 bitů. */
std::uint64_t check_num(std::uint64_t word, int &length, std::uint64_t n) {
    std::uint64_t mask = UINT64_MAX;
    for (int i = 64; i > 0 && mask >= n; i >>= 1, mask <<= i) {
        int iters = 64 / i;
        bool fits = true;
        for (int j = 0; j < iters && fits; j++) {
            if (((mask >> (j * i)) & word) >> ((iters - j - 1) * i) != n) {
                fits = false;

            }
        }
        if (fits) {
            length = i;
            return n;
        }
    }
    return 0;
}

std::uint64_t periodic( std::uint64_t word, int &length ) {
    length = 1;
    if (word == 0) {
        return 0;
    }
    for (std::uint64_t n = 1; n <= word; n++) {
        uint64_t aux = check_num(word, length, n);
        if (aux == n)
            return n;
    }
    return 0;
}

int main() {
    int length;

    assert( periodic( 0x100000001u, length ) == 1 );
    assert( length == 32 );
    assert( periodic( 0x1212121212121212u, length ) == 0x12 );
    assert( length == 8 );
    assert( periodic( 0x1234123412341234u, length ) == 0x1234 );
    assert( length == 16 );
    assert( periodic( 0, length ) == 0 );
    assert( length == 1 );
    assert( periodic( 1, length ) == 1 );
    assert( length == 64 );

    return 0;
}
