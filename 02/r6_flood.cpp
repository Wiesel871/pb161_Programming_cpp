#include <vector>
#include <cassert>

/* V tomto cvičení implementujeme tzv. semínkové vyplňování, obvykle
 * popsané algoritmem, který:
 *
 *  1. dostane na vstupu bitmapu (odélníkovou síť pixelů),
 *  2. počátečnou pozici v síti,
 *  3. barvu výplně (cílovou barvu),
 *
 * a změní celou souvislou jednobarevnou plochu, která obsahuje
 * počáteční pozici, na cílovou barvu.
 *
 * Budeme uvažovat monochromatický případ – pixely jsou černé nebo
 * bílé, resp. 0 nebo 1. Navíc nebudeme měnit vstupní bitmapu, ale
 * pouze spočítáme, kolik políček změní barvu a tuto hodnotu
 * vrátíme. Příklad (prázdná políčka mají hodnotu 0, vybarvená
 * hodnotu 1, startovní pozice má souřadnice 1, 3):
 *
 *   ┌───┬───┬───┬───┬───┬───┐  ┌───┬───┬───┬───┬───┬───┐
 * 0 │   │   │   │░░░│   │   │  │░░░│░░░│░░░│░░░│   │   │
 *   ├───┼───┼───┼───┼───┼───┤  ├───┼───┼───┼───┼───┼───┤
 * 1 │   │░░░│   │░░░│   │   │  │░░░│░░░│░░░│░░░│   │   │
 *   ├───┼───┼───┼───┼───┼───┤  ├───┼───┼───┼───┼───┼───┤
 * 2 │   │░░░│░░░│░░░│░░░│░░░│  │░░░│░░░│░░░│░░░│░░░│░░░│
 *   ├───┼───┼───┼───┼───┼───┤  ├───┼───┼───┼───┼───┼───┤
 * 3 │   │ × │   │░░░│   │   │  │░░░│░×░│░░░│░░░│░░░│░░░│
 *   ├───┼───┼───┼───┼───┼───┤  ├───┼───┼───┼───┼───┼───┤
 * 4 │░░░│░░░│░░░│   │░░░│░░░│  │░░░│░░░│░░░│░░░│░░░│░░░│
 *   ├───┼───┼───┼───┼───┼───┤  ├───┼───┼───┼───┼───┼───┤
 * 5 │   │   │   │   │░░░│   │  │░░░│░░░│░░░│░░░│░░░│   │
 *   └───┴───┴───┴───┴───┴───┘  └───┴───┴───┴───┴───┴───┘
 *     0   1   2   3   4   5      0   1   2   3   4   5
 *
 * Všimněte si, že „záplava“ se šíří i po diagonálách (např.
 * z pozice (2, 3) na (3, 4) a dále na (4, 3)). Dále:
 *
 *  • vstupní bitmapa je zadaná jako jednorozměrný vektor a šířka,
 *  • chybí-li nějaké pixely v posledním řádku, uvažujte jejich
 *    hodnotu nulovou,
 *  • je-li poslední řádek kompletní, nic nepřidávejte,
 *  • je-li startovní pozice, ‹x0› nebo ‹y0›, mimo meze bitmapy
 *    (tzn. její šířku a výšku), žádné pixely barvu nezmění.
 *
 * Poslední parametr, ‹fill›, udává cílovou barvu. Je-li startovní
 * pozice cílové barvy, podobně se nic nebude měnit. */

using grid = std::vector< bool >;

int flood( const grid &pixels, int width,
           int x0, int y0, bool fill );

int main()
{
    // clang-tidy: -modernize-use-bool-literals

    grid small{ 0, 0, 1,
                1, 1, 1,
                1, 0, 0 };

    assert( flood( small, 3, 0, 0, true  ) == 2 );
    assert( flood( small, 3, 1, 0, true  ) == 2 );
    assert( flood( small, 3, 2, 0, true  ) == 0 );
    assert( flood( small, 3, 2, 0, false ) == 5 );
    assert( flood( small, 3, 1, 2, true  ) == 2 );

    grid ex{ 0, 0, 0, 1, 0, 0, 0,
             0, 1, 0, 1, 0, 0, 0,
             0, 1, 1, 1, 1, 1, 1,
             0, 0, 0, 1, 0, 0, 0,
             1, 1, 1, 0, 1, 1, 1,
             0, 0, 0, 0, 1, 1, 1 };

    assert( flood( ex, 7, 1, 3, true  ) == 17 );
    assert( flood( ex, 7, 1, 3, false ) ==  0 );
    assert( flood( ex, 7, 1, 3, false ) ==  0 );
    assert( flood( ex, 7, 5, 0, true  ) ==  6 );

    return 0;
}
