#include <cstddef>
#include <new>
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>

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
 * končí (pro racionální číslo se to jistě stane po kaečném počtu
 * kroků). Hodnotám ⟦a₀, a₁, a₂, …⟧ říkáme «koeficienty» řetězového
 * zlomku – jeho hodnota je jimi jednoznačně určena.
 *
 * Rozmyslete si vhodnou reprezentaci vzhledem k zadanému rozhraní.
 * Je důležité jak to, které operace jsou požadované, tak to, které
 * nejsou. */

struct rat;
struct fraction;

rat make_rat(int, int);
rat base_form(const rat &);

std::vector<int> bi_homographic(
        int, int, int, int,
        int, int, int, int,
        const fraction &, const fraction &);

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


struct fraction {
    std::vector<int> a = {};

    void set_coefficients(const std::vector<int> &v) {
        this->a = v;
    }

    fraction operator+(const fraction &o) {
        return {bi_homographic(0, 1, 1, 0, 0, 0, 0, 1, *this, o)};
    }

    fraction operator*(const fraction &o) {
        return {bi_homographic(1, 0, 0, 0, 0, 0, 0, 1, *this, o)};
    }

    friend auto operator<=>(const fraction &l, const fraction &r) {
        if (auto cmp = l.a[0] <=> r.a[0]; cmp != 0)
            return cmp;
        for (std::size_t i = 1; i < std::min(l.len(), r.len()); i++)
            if (auto cmp = r.a[0] <=> l.a[0]; cmp != 0)
                return cmp;
        return l.len() <=> r.len();
    }

    friend auto operator==(const fraction &l, const fraction &r) {
        return l.a == r.a;
    }

    std::size_t len() const {
        return this->a.size();
    }
};

void extend_vec(std::vector<int> &t, const std::vector<int> &s) {
    t.reserve(t.size() + distance(s.begin(), s.end()));
    t.insert(t.end(), s.begin(), s.end());
}

std::vector<int> cfv(int x) {
    return {x};
}

std::vector<int> cfv(rat x) {
    std::vector<int> res = {};
    while (x.q != 0) {
        res.push_back(static_cast<int>(x.p / std::floor(static_cast<double>(x.q))));
        x = {x.q, x.p % x.q};
    }
    return res; 
}

fraction cf(auto x) {
    return {cfv(x)};
}

std::vector<int> homographic(int a, int b, int c, int d, const fraction &x) {
    std::vector<int> res;
    std::size_t i = 0;
    while (i < x.len()) {
        if (a == c && c == 0) {
            extend_vec(res, cfv({b, d}));
            return res;
        } else if (c && d && std::floor(static_cast<double>(a) / c) == std::floor(static_cast<double>(b) / d)){
            res.push_back(static_cast<int>(std::floor(a / static_cast<double>(c))));
            std::vector<int> aux = {c, d, a % c, b % d};
            a = aux[0], b = aux[1], c = aux[2], d = aux[3];
        } else {
            std::vector<int> aux = {a * x.a[i] + b, a, c * x.a[i] + d, c};
            a = aux[0], b = aux[1], c = aux[2], d = aux[3];
            ++i;
        }
    }
    extend_vec(res, cfv({a, c}));
    return res;
}

std::vector<int> since(const std::vector<int> &s, int i) {
    std::vector<int> res;
    res.reserve(res.size() + distance(s.begin() - i, s.end()));
    res.insert(res.end(), s.begin() + i, s.end());
    return res;
}

std::vector<int> bi_homographic(
        int a, int b, int c, int d, 
        int e, int f, int g, int h, 
        const fraction &x, const fraction &y) {
    std::size_t i = 0, j = 0;
    std::vector<int> res;
    while (i < x.len() && j < y.len()) {
        if (a == b && b == e && e == d && d == 0) {
            extend_vec(res, homographic(c, d, g, h, {since(y.a, j)}));
            return res;
        } else if (a == c && c == e && e == f && f == 0) {
            extend_vec(res, homographic(b, d, f, h, {since(x.a, i)}));
            return res;
        } else if (e && f && g && h) {
            double ae = std::floor(static_cast<double>(a) / e);
            double bf = std::floor(static_cast<double>(b) / f);
            double cg = std::floor(static_cast<double>(c) / g);
            double dh = std::floor(static_cast<double>(d) / h);
            if (ae == bf && bf == cg && cg == dh) {
                res.push_back(a / e);
                std::vector<int> aux = {a % e, b % f, c % g, d % h};
                a = e, b = f, c = g, d = h;
                e = aux[0], f = aux[1], g = aux[2], h = aux[3];
            }
        } else if (f * g * h == 0 || std::abs(b / f - d / h) > std::abs(c / g - d / h)) {
                std::vector<int> aux = {
                    a * x.a[i] + c, b * x.a[i] + d, a, b,
                    e * x.a[i] + g, f * x.a[i] + h, e, f
                };
                a = aux[0], b = aux[1], c = aux[2], d = aux[3];
                e = aux[4], f = aux[5], g = aux[6], h = aux[7];
                ++i;
        } else {
                std::vector<int> aux = {
                    a * y.a[j] + b, a, c * y.a[j] + d, c,
                    e * y.a[j] + f, e, g * y.a[j] + h, g
                };
                a = aux[0], b = aux[1], c = aux[2], d = aux[3];
                e = aux[4], f = aux[5], g = aux[6], h = aux[7];
                ++j;
        }
    }
    if (i == x.len()) {
        extend_vec(res, homographic(a, b, e, f, {since(y.a, j)}));
    } else {
        extend_vec(res, homographic(a, c, e, g, {since(x.a, i)}));
    }
    return res;
}


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
