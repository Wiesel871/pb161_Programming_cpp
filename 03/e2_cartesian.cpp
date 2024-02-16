#include <cassert>

/* V tomto příkladu implementujeme typ ‹cartesian›, který
 * reprezentuje komplexní číslo pomocí reálné a imaginární části.
 * Takto realizovaná čísla umožníme sčítat, odečítat, získat číslo
 * opačné (unárním mínus) a určit jejich rovnost (zamyslete se,
 * má-li smysl definovat na tomto typu uspořádání; proč ano, proč
 * ne?). */

struct cartesian;

/* Implementujte také čistou funkci ‹make_cartesian›, která vytvoří
 * hodnotu typu ‹cartesian› se zadané reálné a imaginární složky. */

cartesian make_cartesian( double, double );

int main()
{
    cartesian zero = make_cartesian( 0, 0 ),
              one = make_cartesian( 1, 0 ),
              i = make_cartesian( 0, 1 );
    const cartesian minus_one = -one, minus_i = -i;

    assert( i + minus_i == zero );
    assert( one - i == make_cartesian( 1, -1 ) );
    assert( i + i == make_cartesian( 0, 2 ) );
    assert( minus_i + i == zero );
    assert( -minus_i == i );
    assert( -minus_one == one );

    return 0;
}
