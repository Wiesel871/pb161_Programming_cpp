#include <cstdint>
#include <climits>
#include <cassert>

/* Předmětem této úlohy jsou «ternární» bitové operace: vstupem jsou
 * 3 čísla a kód operace. Každý bit výsledku je určen příslušnými
 * 3 bity operandů. Tyto 3 vstupní bity lze chápat jako pravdivostní
 * hodnoty – potom je celkem jasné, že operaci můžeme zadat
 * klasickou pravdivostní tabulkou, která pro každou kombinaci
 * 3 bitů určí výsledek. Mohla by vypadat např. takto:
 *
 *  │ a │ b │ c │ výsledek │
 *  ├───┼───┼───│──────────┤
 *  │ 0 │ 0 │ 0 │   ⟦r₀⟧   │
 *  │ 0 │ 0 │ 1 │   ⟦r₁⟧   │
 *  │ 0 │ 1 │ 0 │   ⟦r₂⟧   │
 *  │ 0 │ 1 │ 1 │   ⟦r₃⟧   │
 *  │ 1 │ 0 │ 0 │   ⟦r₄⟧   │
 *  │ 1 │ 0 │ 1 │   ⟦r₅⟧   │
 *  │ 1 │ 1 │ 0 │   ⟦r₆⟧   │
 *  │ 1 │ 1 │ 1 │   ⟦r₇⟧   │
 *
 * Snadno se nahlédne, že zafixujeme-li tuto tabulku a zadáme
 * hodnoty ⟦r₀⟧ až ⟦r₇⟧, kýžená operace bude jednoznačně určena.
 * Protože potřebujeme 8 pravdivostních hodnot, můžeme je například
 * předat jako jednobajtovou hodnotu typu ‹std::uint8_t›. Hodnotu
 * ⟦r₀⟧ předáme v nejnižším a ⟦r₇⟧ v nejvyšším bitu.
 *
 * Operace musí fungovat pro libovolný bezznaménkový celočíselný
 * typ. Můžete předpokládat, že hodnoty ‹a›, ‹b›, ‹c› jsou stejných
 * typů (ale také se můžete zamyslet, jak řešit situaci, kdy stejné
 * nejsou). */

auto bitwise( std::uint8_t opcode, auto a, auto b, auto c );

int main()
{
    assert( bitwise( 0b0000'0000, 3u, 5u, 7u ) == 0 );
    assert( bitwise( 0b0000'0001, 0u, 0u, 0u ) == UINT_MAX );
    assert( bitwise( 0b0000'0010, 0u, 0u, 33u ) == 33u );
    assert( bitwise( 0b1000'0000, 1u, 3u, 7u ) == 1u );
    assert( bitwise( 0b1100'0000, 7u, 3u, 1u ) == 3u );
    assert( bitwise( 0b0001'0000, 7u, 3u, 1u ) == 4u );

    std::uint8_t x = 13;

    assert( bitwise( 0b1111'1111, x, x, x ) == 255 );
    assert( bitwise( 0b0000'0001, 0ul, 0ul, 0ul ) == ULONG_MAX );
    assert( bitwise( 0b0000'0001, 0ul, 0ul, 1ul ) == ULONG_MAX - 1 );

    return 0;
}
