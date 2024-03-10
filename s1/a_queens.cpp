#include <cstdint>
#include <cassert>
#include <iostream>

/* V této úloze budete programovat řešení tzv. problému osmi
 * královen (osmi dam). Vaše řešení bude predikát, kterého vstupem
 * bude jediné 64-bitové bezznaménkové číslo (použijeme typ
 * ‹uint64_t›), které popisuje vstupní stav šachovnice: šachovnice
 * 8×8 má právě 64 polí, a pro reprezentaci každého pole nám stačí
 * jediný bit, který určí, je-li na tomto políčku umístěna královna.
 *
 * Políčka šachovnice jsou uspořádána počínaje levým horním rohem
 * (nejvyšší, tedy 64. bit) a postupují zleva doprava (druhé pole
 * prvního řádku je uloženo v 63. bitu, tj. druhém nejvyšším) po
 * řádcích zleva doprava (první pole druhého řádku je 56. bit),
 * atd., až po nejnižší (první) bit, který reprezentuje pravý dolní
 * roh.
 *
 * Predikát nechť je pravdivý právě tehdy, není-li žádná královna na
 * šachovnici ohrožena jinou. Program musí pracovat správně i pro
 * případy, kdy je na šachovnici jiný počet královen než 8.
 * Očekávaná složitost je v řádu 64² operací – totiž O(n²) kde ⟦n⟧
 * představuje počet políček.
 *
 * Poznámka: preferované řešení používá pro manipulaci se šachovnicí
 * pouze bitové operace a zejména nepoužívá standardní kontejnery.
 * Řešení, které bude nevhodně používat kontejnery (spadá sem např.
 * jakékoliv použití ‹std::vector›) nemůže získat známku A. */
const uint64_t ONE = 0b1lu;



bool queens( std::uint64_t board ) {
    uint8_t cols = 0, rows = 0;
    uint64_t cross1 = 0, cross2 = 0;
    for (uint64_t r = 0; r < 8; ++r) {
        for (uint64_t c = 0; c < 8; ++c) {
            if (!(board & (ONE << (8 * r + c))))
                continue;
            if (rows & (ONE << r) 
            ||  cols & (ONE << c) 
            ||  cross1 & (ONE << (c + r)) 
            ||  cross2 & (ONE << (7 + r - c)))
                return false;
            rows    |= ONE << r;
            cols    |= ONE << c;
            cross1  |= ONE << (c + r);
            cross2  |= ONE << (7 + r - c);

        }
    }
    return true;
}

int main()
{
    assert( queens( 0lu ) );
    assert(queens(1));
    assert(queens(2));
    assert( !queens( 3lu ) );
    assert(!queens(0x4080lu));
    assert(!queens(0x201));
    assert(!queens(0x8000000000000001));
    assert(!queens(0x8001000000008001));
    assert(queens(0x8008010420024010));
    assert(queens(0x8004012002104008));
    assert(!queens(0x8000800000000000));
    return 0;
}
