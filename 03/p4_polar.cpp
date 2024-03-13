#include <cmath>
#include <cassert>

/* Vaším úkolem je implementovat typ ‹polar›, který realizuje
 * polární reprezentaci komplexního čísla. Protože tato podoba
 * zjednodušuje násobení a dělení, implementujeme v této úloze právě
 * tyto operace (sčítání jsme definovali v příkladu ‹e2_cartesian›).
 *
 * Krom násobení a dělení nechť je možné pro hodnoty typu polar
 * určit jejich rovnost operátory ‹==› a ‹!=›.  Rovnost
 * implementujte s ohledem na nepřesnost aritmetiky s plovoucí
 * desetinnou čárkou. V tomto příkladě můžete pro reálná čísla (typu
 * ‹double›) místo ‹x == y› použít ‹std::fabs( x - y ) < 1e-10›.
 *
 * Pozor! Argument komplexního čísla je «periodický»: buďto jej
 * normalizujte tak, aby ležel v intervalu ⟦[0, 2π)⟧, nebo
 * zajistěte, aby platilo ‹polar( 1, x ) == polar( 1, x + 2π )›. */
struct polar;

polar make_polar(double, double);


struct polar {
    double r, d;

    polar operator*(const polar &r) {
        return make_polar(this->r * r.r, this->d + r.d);
    }

    polar operator/(const polar &r) {
        return make_polar(this->r / r.r, this->d - r.d);
    }

    friend bool operator==(const polar &l, const polar &r) {
        return
            (l.r == 0.0 && r.r == 0.0) ||
                (std::fabs(l.r - r.r) < 0.001 &&
                std::fabs(l.d - r.d) < 0.001);
    }
};

polar make_polar( double r, double d) {
    return {r, d - (std::floor(d / 6.283) * 6.283)};
}

int main()
{
    polar zero = make_polar( 0, 0 ),
          one = make_polar( 1, 0 ),
          i = make_polar( 1, 2 * std::atan( 1 ) ),
          minus_one = make_polar( 1, 4 * std::atan( 1 ) );

    assert( zero * one == zero );
    assert( zero * i == zero );
    assert( one * i == i );
    assert( i * i == minus_one );
    assert( i / i == one );
    assert( one / i == minus_one * i );
    assert( minus_one * minus_one == one );
    assert( minus_one / minus_one == one );

    return 0;
}
