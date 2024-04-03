#include <cassert>
#include <exception>
#include <utility>
#include <cmath>
#include <cstdio>

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
struct no_solution : std::exception {

};

bool eq(double a, double b) {
    return std::fabs(a - b) <= ( (std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * 1.0e-5f);
}

struct row {
    double x, y, d;

    row(double d) : d{d} {}

    row(double x, double y, double d) : x{x}, y{y}, d{d} {}

    row operator+(const row &r) const {
        return {x + r.x, y + r.y, d + r.d};
    }

    row operator+(int r) const {
        return {x, y, d + r};
    }

    row operator+(double r) const {
        return {x, y, d + r};
    }

    row operator==(const row &r) const {
        return {x - r.x, y - r.y, d - r.d};    
    }

    row operator==(int r) const {
        return {x, y, d - r};
    }

    row operator==(double r) const {
        return {x, y, d - r};
    }

    row operator-(const row &r) const {
        return {x - r.x, y - r.y, d - r.d};    
    }

    row operator-(int r) const {
        return {x, y, d - r};
    }

    row operator-(double r) const {
        return {x, y, d - r};
    }

    row operator*(double r) const {
        return {r * x, r * y, r * d};
    }

    row operator*(int r) const {
        return {r * x, r * y, r * d};
    }

    friend row operator*(double l, const row &r) {
        return {l * r.x, l * r.y, l * r.d};
    }

    friend row operator*(int l, const row &r) {
        return {l * r.x, l * r.y, l * r.d};
    }

    row operator/(double r) const {
        return {x / r, y / r, d / r};
    }

    row operator/(int r) const {
        return {x / r, y / r, d / r};
    }
};

const row x {1, 0, 0};
const row y {0, 1, 0};

inline void check(const row &r) {
    if (eq(r.x, 0) && eq(r.y, 0) && !eq(r.d, 0))
        throw no_solution{};
}
std::pair<double, double> solve(const row &r1, const row &r2) {
    check(r1);
    check(r2);
    std::pair<double, double> res;
    double det_a = (r1.x * r2.y - r1.y * r2.x);
    if (eq(det_a, 0))
        throw no_solution{};
    double det_a1 = (r1.d * r2.y - r1.y * r2.d);
    double det_a2 = (r1.x * r2.d - r1.d * r2.x);
    res.first = det_a1 / det_a;
    res.second = det_a2 / det_a;
    if (det_a < 0) {
        if (eq(det_a1, 0) && eq(det_a2, 0)) {
            res.first = 0;
            res.second = 0;
        } else if (eq(det_a1, 0)) {
            res.first = 0;
            res.second = -res.second;
        } else if (eq(det_a2, 0)) {
            res.first = -res.first;
            res.second = 0;
        } else if (det_a1 > 0) {
            res.first = -res.first;
            res.second = -res.second;
        }
    } else if (det_a1 <= 0) {
        res.first = -res.first;
        res.second = -res.second;
    }

    return res;
}

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
