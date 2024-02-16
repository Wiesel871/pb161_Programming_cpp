#include <vector>
#include <cassert>

/* Předmětem tohoto příkladu jsou tzv. «řetězové zlomky». Typ
 * ‹fraction› bude reprezentovat racionální číslo, které lze:
 *
 *  • zadat posloupností koeficientů řetězového zlomku (přesněji
 *    popsáno níže) pomocí metody ‹set_coefficients( cv )› kde ‹cv›
 *    je vektor hodnot typu ‹int›,
 *  • sčítat (operátorem ‹+›),
 *  • násobit (operátorem ‹*›),
 *  • srovnávat (všemi relačními operátory, tzn. ‹==›, ‹!=›, ‹<›,
 *    ‹<=›, ‹>›, ‹>=›).
 *
 * Řetězový zlomek reprezentujej racionální číslo ⟦q₀⟧ jako součet
 * celého čísla ⟦a₀⟧ a převrácené hodnoty nějakého dalšího
 * racionálního čísla, ⟦q₁⟧, které je samo zapsáno pomocí řetězového
 * zlomku. Tedy
 *
 *  ⟦ q₀ = a₀ + 1/q₁
 *    q₁ = a₁ + 1/q₂
 *    q₂ = a₂ + 1/q₃ ⟧
 *
 * a tak dále, až než je nějaké ⟦qᵢ⟧ celé číslo, kterým sekvence
 * končí (pro racionální číslo se to jistě stane po konečném počtu
 * kroků). Hodnotám ⟦a₀, a₁, a₂, …⟧ říkáme «koeficienty» řetězového
 * zlomku – jeho hodnota je jimi jednoznačně určena.
 *
 * Rozmyslete si vhodnou reprezentaci vzhledem k zadanému rozhraní.
 * Je důležité jak to, které operace jsou požadované, tak to, které
 * nejsou. */

struct fraction;

int main()
{
    const std::vector c_1{ 1 },
                      c_2{ 4, 2, 6, 7 },
                      c_3{ 3, 4, 12, 4 },
                      c_4{ 0, 2, 4 },
                      c_5{ 0, 4, 2, 6, 7 };

    fraction f_1, f_2, f_3, f_4, f_5;

    f_1.set_coefficients( c_1 );
    f_2.set_coefficients( c_2 );
    f_3.set_coefficients( c_3 );
    f_4.set_coefficients( c_4 );
    f_5.set_coefficients( c_5 );

    assert( f_2 > f_1 );
    assert( f_3 > f_1 );
    assert( f_3 < f_1 + f_1 + f_1 + f_1 );
    assert( f_4 < f_1 );
    assert( f_4 + f_4 + f_4 > f_1 );
    assert( f_2 * f_5 == f_1 );

    return 0;
}
