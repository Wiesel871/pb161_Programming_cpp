#include <cassert>
#include <cmath>

/* V této úloze se budeme pohybovat v dvourozměrné ploše a počítat
 * při tom uraženou vzdálenost. Typ ‹walk› nechť má tyto metody:
 *
 *  • ‹line( p )› – přesuneme se do bodu ‹p› po úsečce,
 *  • ‹arc( p, radius )› – přesuneme se do bodu ‹p› po kružnicovém
 *    oblouku s poloměrem ‹radius›,¹ přitom ‹radius› je alespoň
 *    polovina vzdálenosti do bodu ‹p› po přímce,
 *  • ‹backtrack()› – vrátíme se po vlastních stopách do předchozího
 *    bodu (vzdálenost se přitom bude «zvětšovat»),
 *  • ‹distance()› – vrátí celkovou dosud uraženou vzdálenost.
 *
 * Metody nechť je možné libovolně řetězit, tzn. je-li ‹w› typu
 * ‹walk›, následovný výraz musí být dobře utvořený:
 *
 *     w.line( { 1, 1 } )
 *      .line( { 2, 1 } )
 *      .backtrack()
 *      .arc( { 4, 1 }, 7 );
 *
 * Hodnoty typu ‹walk› lze sestrojit zadáním počátečního bodu, nebo
 * implicitně – začínají pak z bodu ⟦(0, 0)⟧.
 *
 * ¹ Potřebný středový úhel naleznete například vyřešením
 *   rovnoramenného trojúhelníku s délkou ramene ‹radius› a
 *   základnou určenou vzdáleností spojovaných bodů. */
struct point {
    double x = 0, y = 0;

    double line (const point &r) {
        return std::sqrt(std::pow(x - r.x, 2) + std::pow(y - r.y, 2));
    }
};

struct walk {
    point pos = {};
    point prev = {};
    double _distance = 0.0;
    double last_step = 0.0;

    walk() = default;

    walk(const point &p) : pos{p} {};

    double distance() const {
        return _distance;
    }

    walk &line(const point &p) {
        last_step = pos.line(p);
        prev = pos;
        _distance += last_step;
        pos = p;
        return (*this);
    }

    walk &backtrack() {
        _distance += last_step;
        std::swap(pos, prev);
        return (*this);
    }

    walk &arc(const point &p, double r) {
        double delta = 2 * std::asin(pos.line(p) / (2 * r));
        last_step = r * delta;
        _distance += last_step;
        prev = pos;
        pos = p;
        return (*this);
    }

};

int main()
{
    walk a;
    const walk &ca = a;
    assert( ca.distance() == 0 );

    auto close = []( double a, double b )
    {
        return std::fabs( a - b ) < 1e-10;
    };

    a.line( { 3, 0 } );
    assert( close( ca.distance(), 3 ) );
    assert( close( ca.distance(), 3 ) );

    assert( close( a.line( { 3, 4 } ).distance(), 7 ) );
    assert( close( a.line( { 3, 2 } ).line( { 8, 2 } ).distance(), 14 ) );

    walk b( { 3, 2 } );
    assert( b.distance() == 0 );

    b.arc( { 6, 6 }, 3 );
    assert( close( b.distance(), 5.9106647 ) );
    assert( close( b.arc( { 8, 6 }, 2 ).distance(), 8.0050598024 ) );

    walk c;
    assert( close( c.line( { 2, 0 } ).backtrack().distance(), 4 ) );
    c.line( { 0, 1 } ).line( { -3, 1 } ).line( { -3, 4 } );
    assert( close( c.backtrack().backtrack().distance(), 17 ) );

    walk d;
    d.line( { 1, 1 } )
     .line( { 2, 1 } )
     .backtrack()
     .arc( { 4, 1 }, 7 );

    assert( close( d.distance(), 6.4376605439 ) );

    return 0;
}
