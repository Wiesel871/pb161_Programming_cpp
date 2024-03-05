#include <cstddef>
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
 * končí (pro racionální číslo se to jistě stane po konečném počtu
 * kroků). Hodnotám ⟦a₀, a₁, a₂, …⟧ říkáme «koeficienty» řetězového
 * zlomku – jeho hodnota je jimi jednoznačně určena.
 *
 * Rozmyslete si vhodnou reprezentaci vzhledem k zadanému rozhraní.
 * Je důležité jak to, které operace jsou požadované, tak to, které
 * nejsou. */

void printv(const std::vector<int> &v) {
    for (int x: v) 
        std::cout << x << " ";
    std::cout << std::endl;
}

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


struct fraction {
    std::vector<int> a = {};

    void set_coefficients(const std::vector<int> &v) {
        this->a = v;
    }

    fraction operator+(const fraction &o) {
        std::cout << "in" << std::endl;
        printv(this->a);
        printv(o.a);
        std::vector<int> res;
        int carry = 0;
        int st = 0;
        for (std::size_t i = 0; i < std::max(this->len(), o.len()); i++) {
            st = 
                (i < this->len() ? this->a[i] : 0) + 
                (i < o.len() ? o.a[i] : 0) + 
                carry;
            carry = st / 10;
            res.push_back(st % 10);
        }
        if (carry != 0)
            res.push_back(carry);
        std::cout << "out" << std::endl;
        printv(res);
        return {res};
    }

    fraction operator*(const fraction &o) {
        return o;
    }

    friend auto operator<=>(const fraction &l, const fraction &r) {
        std::cout << "cmp" << std::endl;
        printv(l.a);
        printv(r.a);
        if (auto cmp = l.a[0] <=> r.a[0]; cmp != 0)
            return cmp;
        for (std::size_t i = 1; i < std::min(l.len(), r.len()); i++)
            if (auto cmp = r.a[0] <=> l.a[0]; cmp != 0)
                return cmp;
        return l.len() <=> r.len();
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
    if (x.q == 0)
       return {};
    std::vector<int> res = {static_cast<int>(x.p / std::floor(static_cast<double>(x.q)))};
    extend_vec(res, cfv({.p = x.q, .q = x.p % x.q}));
    return res; 
}

fraction cf(auto x) {
    return {cfv(x)};
}

std::vector<int> homographic(int a, int b, int c, int d, fraction x) {
    if (x.len() == 0)
        return cfv({a, c});
    if (a == c && c == 0)
        return cfv({b, d});
    if (c && d && std::floor(static_cast<double>(a) / c) == std::floor(static_cast<double>(b) / d)) {
        std::vector<int> res = {static_cast<int>(std::floor(a / static_cast<double>(c)))};
        extend_vec(res, homographic(c, d, a % c, b % d, x));
        return res;
    }
    int fst = x.a[0];
    std::vector<int> rest = {};
    rest.insert(rest.begin(), x.a.begin() + 1, x.a.end());
    return homographic(a * fst + b, a, c * fst + d, c, {rest});
}


int main()
{
    const std::vector c_1{ 1 },
                      c_2{ 4, 2, 6, 7 },
                      c_3{ 3, 4, 12, 4 },
                      c_4{ 0, 2, 4 },
                      c_5{ 0, 4, 2, 6, 7 };

    fraction f_1, f_2, f_3, f_4, f_5;

    std::cout << "balls" << "\n";
    printv(homographic(2, 0, 0, 1, cf(rat {17, 6})));

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
    //assert( f_2 * f_5 == f_1 );

    return 0;
}
