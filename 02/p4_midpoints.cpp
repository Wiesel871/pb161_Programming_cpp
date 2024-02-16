#include <vector>
#include <cmath>
#include <cassert>

/* Strukturu ‹point› doplňte tak, aby měla složky ‹x› a ‹y›, kde
 * obojí jsou čísla s plovoucí desetinnou čárkou, a to tak, že
 * deklarace ‹point p;› vytvoří bod se souřadnicemi ⟦0, 0⟧. */

struct point;

/* Nyní uvažme uzavřenou lomenou čáru. Nahraďte každou úsečku A
 * takovou, která začíná prostředním bodem úsečky A a končí
 * prostředbním bodem úsečky B, kde B v obrazci následuje
 * bezprostředně po A. Vstup je zadán jako sekvence bodů (kde každý
 * bod náleží dvěma úsečkám). Poslední úsečka jde z posledního bodu
 * do prvního, čím se obrazec uzavře. */

void midpoints( std::vector< point > &pts );

int main()
{
    auto near1d = []( double a, double b )
    {
        return std::fabs( a - b ) < 1e-8;
    };

    auto near = [&]( point a, point b )
    {
        return near1d( a.x, b.x ) && near1d( a.y, b.y );
    };

    using point_vec = std::vector< point >;

    point_vec s1{ { 0, 0 }, { 4, 0 }, { 4, 3 } };
    midpoints( s1 );

    assert( near( s1[ 0 ], point{ 2, 0 } ) );
    assert( near( s1[ 1 ], point{ 4, 1.5 } ) );
    assert( near( s1[ 2 ], point{ 2, 1.5 } ) );

    point_vec s2{ { 0, 0 }, { 0, 4 }, { 4, 4 }, { 4, 0 } };
    midpoints( s2 );

    assert( near( s2[ 0 ], point{ 0, 2 } ) );
    assert( near( s2[ 1 ], point{ 2, 4 } ) );
    assert( near( s2[ 2 ], point{ 4, 2 } ) );
    assert( near( s2[ 3 ], point{ 2, 0 } ) );

    return 0;
}
