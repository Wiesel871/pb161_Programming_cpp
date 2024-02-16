#include <cassert>
#include <cmath>

/* Doplňte definice typů ‹point›, ‹polygon› a ‹circle› tak, abyste
 * pak mohli s jejich pomocí možné implementovat tyto čisté funkce:
 *
 *  • ‹make_polygon›, která přijme jako parametr celé číslo (počet
 *    stran) a dále:
 *    ◦ 2 body (střed a některý vrchol), nebo
 *    ◦ 1 bod (střed) a 1 reálné číslo (poloměr opsané kružnice),
 *  • ‹make_circle› které přijme jako parametry:
 *    ◦ 2 body (střed a bod na kružnici), nebo
 *    ◦ 1 bod a 1 reálné číslo (střed a poloměr),
 *  • ‹area›, které přijme ‹polygon› nebo ‹circle› a vrátí plochu
 *    odpovídajícího útvaru.
 *
 * Typ ‹point› nechť má složky ‹x› a ‹y› (reálná čísla). */

struct point;
struct polygon;
struct circle;

int main()
{
    point p00{ .x = 0, .y = 0 },
          p01{ .x = 0, .y = 1 },
          p10{ .x = 1, .y = 0 },
          p11{ .x = 1, .y = 1 },
          p20{ .x = 2, .y = 0 };

    polygon square = make_polygon( 4, p00, std::sqrt( 2 ) );
    assert( std::fabs( area( square ) - 4 ) < .001 );

    polygon hexagon = make_polygon( 6, p00, p01 );
    assert( std::fabs( area( hexagon ) - 2.598 ) < .001 );

    circle c1 = make_circle( p00, p10 );
    circle c2 = make_circle( p00, p20 );
    circle c3 = make_circle( p00, p11 );

    assert( std::fabs( area( c1 ) -     3.1415 ) < .001 );
    assert( std::fabs( area( c2 ) - 4 * 3.1415 ) < .001 );
    assert( std::fabs( area( c3 ) - 2 * 3.1415 ) < .001 );

    return 0;
}
