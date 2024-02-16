#include <cassert>
#include <vector>

int counter = 0;

/* Přidejte konstruktory a destruktor typu ‹counted› tak, aby
 * počítadlo ‹counter› vždy obsahovalo počet existujících hodnot
 * typu ‹counted›. Nezapomeňte na pravidlo pěti (rule of five). */

struct counted;

int main()
{
    assert( counter == 0 );
    counted c_1;
    assert( counter == 1 );

    {
        counted c_2;
        assert( counter == 2 );

        counted c_3 = c_2;
        assert( counter == 3 );

        std::vector< counted > vec;
        vec.emplace_back();
        vec.emplace_back();

        assert( counter == 5 );
    }

    assert( counter == 1 );

    return 0;

    // clang-tidy: -performance-unnecessary-copy-initialization
}
