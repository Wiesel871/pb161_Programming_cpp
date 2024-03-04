#include <cassert>
#include <cmath>
#include <compare>
#include <new>
#include <iostream>

/* V tomto příkladu budeme programovat jednoduchá racionální čísla
 * (taková, že je lze reprezentovat dvojicí celých čísel typu
 * ‹int›). Hodnoty typu ‹rat› lze:
 *
 *  • vytvořit čistou funkcí ‹make_rat( p, q )› kde ‹p›, ‹q› jsou
 *    hodnoty typu ‹int› (čitatel a jmenovatel) a ‹q > 0›,
 *  • použít jako operandy základních aritmetických operací:
 *    ◦ sčítání ‹+›,
 *    ◦ odečítání (‹-›),
 *    ◦ násobení (‹*›) a
 *    ◦ dělení (‹/›),
 *  • libovolně srovnávat (‹==›, ‹!=›, ‹<=›, ‹<›, ‹>›, ‹>=›).
 *
 * Vzpomeňte si, jak se jednotlivé operace nad racionálními čísly
 * zavádí. Jsou-li ⟦a = p₁/q₁⟧ a ⟦b = p₂/q₂⟧ zlomky v základním
 * tvaru, můžete se u libovolné operace ⟦a ⋄ b⟧ spolehnout, že
 * žádný ze součinů ⟦p₁⋅q₂⟧, ⟦p₂⋅q₁⟧, ⟦p₁⋅p₂⟧ a ⟦q₁⋅q₂⟧ nepřeteče
 * rozsah ‹int›-u. */

struct rat;

rat make_rat(int, int);
rat base_form(const rat &);

struct rat {
    int p, q;

    friend rat operator+(rat l, const rat &r) {
        return make_rat(l.p * r.q + r.p * l.q, l.q * r.q);
    }

    friend rat operator-(rat l, const rat &r) {
        return make_rat(l.p * r.q - r.p * l.q, l.q * r.q);
    }

    friend rat operator*(rat l, const rat &r) {
        return make_rat(l.p * r.p, l.q * r.q);
    }

    friend rat operator/(rat l, const rat &r) {
        return make_rat(l.p * r.q, l.q * r.p);
    }

    friend bool operator==(const rat &l, const rat &r) {
        const rat lb = base_form(l);
        const rat rb = base_form(r);
        return lb.p == rb.p && lb.q == rb.q;
    }

    friend auto operator<=>(const rat &l, const rat &r) {
        const rat lb = make_rat(l.p, l.q);
        const rat rb = make_rat(r.p, r.q);
        return (lb.p * rb.q) <=> (rb.p * lb.q);
    }
};

rat base_form(const rat &r) {
    if (r.q == 1)
        return r;
    if (r.p % r.q == 0)
        return {r.p / r.q, 1};
    if (r.q % r.p == 0) {
        if (r.p < 0)
            return {-1, std::abs(r.q / r.p)};
        return {1, r.q / r.p};
    }
    rat res = r;
    for (int i = 2; i < std::sqrt(std::min(res.q, res.p)); i += 1 + (i != 2)) {
        while (res.p % i == 0 && res.q % i == 0) {
            res.p /= i;
            res.q /= i;
        }
    }
    return res;
}

rat make_rat(int p, int q) {
    assert(q != 0);
    if (q < 0)
        return base_form({-p, -q});
    return base_form({p, q});
}



rat make_rat( int, int );

int main()
{
    rat one = make_rat( 1, 1 ),
        half = make_rat( 1, 2 ),
        three_halves = make_rat( 3, 2 ),
        minus_half = make_rat( -1, 2 ),
        third = make_rat( 1, 3 ),
        two = make_rat( 2, 1 ),
        minus_sixth = make_rat( -1, 6 );

    assert( half < one );
    assert( one < three_halves );
    assert( half < three_halves );

    assert( one + one == two );
    assert( two / two == one );
    assert( one * two == two );
    assert( one != two );
    assert( !( one == two ) );
    assert( !( one != one ) );
    assert( third * ( one + two ) == one );
    assert( half + half == one );
    assert( half + one == three_halves );
    assert( one < one + one );
    assert( minus_half + minus_half < minus_half );
    assert( minus_half < half );
    assert( minus_half + half == one - one );
    assert( minus_half * minus_half == half / two );
    assert( three_halves + minus_half == one );
    assert( half + third < one );
    assert( minus_half < minus_sixth );
    assert( minus_half + third == minus_sixth );

    return 0;
}
