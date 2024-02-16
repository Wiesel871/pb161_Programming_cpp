#include <cassert>
#include <exception>

/* † Napište program, který bude řešit systémy lineárních rovnic
 * o dvou neznámých. Rozhraní bude lehce nekonvenční: přetěžte
 * operátory ‹+›, ‹*› a ‹==› a definujte globální konstanty ‹x› a
 * ‹y› vhodných typů tak, aby bylo možné rovnice zapisovat způsobem,
 * který vidíte níže v proceduře ‹main›. */

/* Uvědomte si, že návratový typ ‹==› nemusí být ‹bool› – naopak,
 * může se jednat o libovolný typ, včetně vlastních. Pro samotnou
 * implementaci funkce ‹solve› doporučujeme použít Cramerovo
 * pravidlo. */

/* Nemá-li zadaný systém řešení, funkce ‹solve› nechť skončí
 * výjimkou ‹no_solution› (tuto odvoďte od ‹std::exception›). Má-li
 * řešení více, je jedno které vrátíte.¹ */

/* ¹ Jsou-li oba determinanty pomocných matic ⟦A₁, A₂⟧ nulové,
 *   systém má libovolně mnoho řešení. Dejte si ale při jejich
 *   vyčíslování pozor na dělení nulou. */

int main()
{
    std::pair a( 1.0, 0.0 ), b( -1.0, 0.0 ), c( .6, -.2 );
    assert( solve(     x + y == 1,  x     - y ==  1 ) == a );
    assert( solve(     x + y == -1, x     - y == -1 ) == b );
    assert( solve(     x + y == x,  x     + y ==  1 ) == a );
    assert( solve( 2 * x + y == 1,  x - 2 * y ==  1 ) == c );
    assert( solve(     x + y - 1,   x     - y - 1   ) == a );

    try
    {
        solve( x + y == 1,  x + y == 2 );
        assert( 0 );
    }
    catch ( const std::exception &e ) {}

    try
    {
        solve( x + y == 1,  x + y == 2 );
        assert( 0 );
    }
    catch ( const no_solution &e ) {}

    return 0;
}
