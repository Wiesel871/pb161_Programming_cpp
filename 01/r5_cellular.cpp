#include <cassert>
#include <cstdint>

/* Napište čistou funkci, která simuluje jeden krok výpočtu
 * jednorozměrného buněčného automatu (cellular automaton). Stav
 * budeme reprezentovat celým číslem bez znaménka – jednotlivé buňky
 * budou bity tohoto čísla. Stav osmibitového automatu by mohl
 * vypadat například takto:
 *
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │ 0 │ 1 │ 1 │ 0 │ 1 │ 0 │ 0 │ 1 │
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *     7   6   5   4   3   2   1   0
 *
 * Pro zjednodušení použijeme pevnou sadu pravidel (+1, 0, -1 jsou
 * relativní pozice bitů vůči tomu, který právě počítáme):
 *
 *  │ +1 │  0 │ -1 │ nová │
 *  ├────┼────┼────│──────┤
 *  │  0 │  0 │  0 │    1 │
 *  │  0 │  0 │  1 │    0 │
 *  │  0 │  1 │  0 │    0 │
 *  │  0 │  1 │  1 │    0 │
 *  │  1 │  0 │  0 │    1 │
 *  │  1 │  0 │  1 │    0 │
 *  │  1 │  1 │  0 │    1 │
 *  │  1 │  1 │  1 │    0 │
 *
 * Pravidla určují, jakou hodnotu bude mít buňka v následujícím
 * stavu, v závislosti na okolních buňkách stavu nynějšího. Na
 * krajích stavu interpretujeme chybějící políčko jako nulu.
 *
 * Výpočet s touto sadou pravidel tedy funguje takto:
 *
 *     ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *     │░0░│░1░│ 1 │ 0 │ 0 │ 0 │ 1 │ 0 │
 *  1  ├───┼───┼───┼───┼───┼───┼───┼───┤  001 → 0
 *     │░0░│   │   │   │   │   │   │   │
 *     └───┴───┴───┴───┴───┴───┴───┴───┘
 *     ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *     │░0░│░1░│░1░│ 0 │ 0 │ 0 │ 1 │ 0 │
 *  2  ├───┼───┼───┼───┼───┼───┼───┼───┤  011 → 0
 *     │ 0 │░0░│   │   │   │   │   │   │
 *     └───┴───┴───┴───┴───┴───┴───┴───┘
 *     ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *     │ 0 │░1░│░1░│░0░│ 0 │ 0 │ 1 │ 0 │
 *  3  ├───┼───┼───┼───┼───┼───┼───┼───┤  110 → 1
 *     │ 0 │ 0 │░1░│   │   │   │   │   │
 *     └───┴───┴───┴───┴───┴───┴───┴───┘
 *
 * atd.
 *
 *     ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *     │ 0 │ 1 │ 1 │ 0 │ 0 │░0░│░1░│░0░│
 *  7  ├───┼───┼───┼───┼───┼───┼───┼───┤  010 → 0
 *     │ 0 │ 0 │ 1 │ 1 │ 1 │ 0 │░0░│   │
 *     └───┴───┴───┴───┴───┴───┴───┴───┘
 *     ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *     │ 0 │ 1 │ 1 │ 0 │ 0 │ 0 │░1░│░0░│
 *  8  ├───┼───┼───┼───┼───┼───┼───┼───┤  100 → 1
 *     │ 0 │ 0 │ 1 │ 1 │ 1 │ 0 │ 0 │░1░│
 *     └───┴───┴───┴───┴───┴───┴───┴───┘
 *
 * Výpočet kroku by mělo být možné provést na libovolně širokém
 * celočíselném typu. */

auto cellular_step( auto w );

int main()
{
    using std::uint8_t, std::uint16_t, std::uint32_t;

    auto step_8b  = []( uint8_t w ) { return cellular_step( w ); };
    auto step_16b = []( uint16_t w ) { return cellular_step( w ); };
    auto step_32b = []( uint32_t w ) { return cellular_step( w ); };

    assert( step_8b( 0b01100010u ) == 0b00111001u );
    assert( step_8b( 0b01100011u ) == 0b00111001u );
    assert( step_8b( 0b01100111u ) == 0b00110001u );
    assert( step_8b( 0b10000000u ) == 0b01111111u );
    assert( step_8b( 0b01000000u ) == 0b00111111u );

    assert( step_16b( 0xffff ) == 1u );
    assert( step_16b( 0xfffe ) == 3u );
    assert( step_32b( 0xffff ) == 0xfffe0001u );
    assert( step_32b( 0xfffe ) == 0xfffe0003u );

    return 0;
}
