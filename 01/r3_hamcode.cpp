/* V této úloze budeme programovat dekodér pro kód Hamming(8, 4) –
 * jedná se o variaci běžnějšího Hamming(7, 4) s dodatečným paritním
 * bitem.
 *
 * Vstupní blok je zadán osmibitovým číslem bez znaménka:
 *
 *   ┌────┬────┬────┬────┬────┬────┬────┬────┐
 *   │ p₀ │ p₁ │ p₂ │ d₁ │ p₃ │ d₂ │ d₃ │ d₄ │
 *   └────┴────┴────┴────┴────┴────┴────┴────┘
 *     7    6    5    4    3    2    1    0
 *
 * Blok je správně utvořený, platí-li tyto vztahy:
 *
 *   ⟦ p₁ = P(d₁, d₂, d₄)
 *     p₂ = P(d₁, d₃, d₄)
 *     p₃ = P(d₂, d₃, d₄)
 *     p₀ = P(p₁, p₂, p₃, d₁, d₂, d₃, d₄) ⟧
 *
 * kde ⟦P⟧ značí «paritu». Graficky (zvýrazněný bit je paritním pro
 * vyznačené bity):
 *
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │▒▒▒│░░░│░░░│░░░│░░░│░░░│░░░│░░░│
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │   │▒▒▒│   │░░░│   │░░░│   │░░░│
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │   │   │▒▒▒│░░░│   │   │░░░│░░░│
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │   │   │   │   │▒▒▒│░░░│░░░│░░░│
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *
 * Rozmyslete si, jaký mají uvedené vztahy dopad na paritu «celých»
 * označených oblastí. Poté napište funkci ‹h84_decode›, která na
 * vstupu dostane jeden blok, ověří správnost paritních bitů, a
 * je-li vše v pořádku, vrátí ‹true› a do výstupního parametru ‹out›
 * zapíše dekódovanou půlslabiku (d₄ v nejnižším bitu). Jinak vrátí
 * ‹false› a hodnotu ‹out› nemění. */

bool h84_decode( std::uint8_t data, std::uint8_t &out );

int main()
{
    std::uint8_t decoded = 11;

    assert( !h84_decode( 0b0000'0001, decoded ) && decoded == 11 );
    assert( !h84_decode( 0b0000'0010, decoded ) && decoded == 11 );
    assert( !h84_decode( 0b0100'0010, decoded ) && decoded == 11 );
    assert( !h84_decode( 0b0001'0010, decoded ) && decoded == 11 );

    assert(  h84_decode( 0b0000'0000, decoded ) && decoded ==  0 );
    assert(  h84_decode( 0b0110'1001, decoded ) && decoded ==  1 );
    assert(  h84_decode( 0b1010'0101, decoded ) && decoded ==  5 );
    assert(  h84_decode( 0b1100'0011, decoded ) && decoded ==  3 );
    assert(  h84_decode( 0b1001'0110, decoded ) && decoded == 14 );
    assert(  h84_decode( 0b1111'1111, decoded ) && decoded == 15 );

    assert( !h84_decode( 0b1111'1101, decoded ) );
    assert( !h84_decode( 0b1110'1101, decoded ) );
    assert( !h84_decode( 0b0111'1101, decoded ) );

    return 0;
}
