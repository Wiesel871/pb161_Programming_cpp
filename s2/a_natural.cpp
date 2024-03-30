#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>


/* Tento úkol rozšiřuje ‹s1/f_natural› o tyto operace (hodnoty ‹m› a
 * ‹n› jsou typu ‹natural›):
 *
 *  • konstruktor, který přijme nezáporný parametr typu ‹double› a
 *    vytvoří hodnotu typu ‹natural›, která reprezentuje dolní
 *    celou část parametru,
 *  • operátory ‹m / n› a ‹m % n› (dělení a zbytek po dělení;
 *    chování pro ‹n = 0› není definované),
 *  • metodu ‹m.digits( n )› která vytvoří ‹std::vector›, který bude
 *    reprezentovat hodnotu ‹m› v soustavě o základu ‹n› (přitom
 *    nejnižší index bude obsahovat nejvýznamnější číslici),
 *  • metodu ‹m.to_double()› která vrátí hodnotu typu ‹double›,
 *    která co nejlépe aproximuje hodnotu ‹m› (je-li ⟦l = log₂(m) -
 *    52⟧ a ‹d = m.to_double()›, musí platit ‹m - 2ˡ ≤ natural( d )›
 *    a zároveň ‹natural( d ) ≤ m + 2ˡ›; je-li ‹m› příliš velké, než
 *    aby šlo typem ‹double› reprezentovat, chování je
 *    nedefinované).
 *
 * Převody z/na typ ‹double› musí být lineární v počtu bitů
 * operandu. Dělení může mít složitost nejvýše kvadratickou v počtu
 * bitů levého operandu. Metoda ‹digits› smí vůči počtu bitů ‹m›,
 * ‹n› provést nejvýše lineární počet «aritmetických operací» (na
 * hodnotách ‹m›, ‹n›). */

const uint64_t UP   = 0xffffffff00000000;
const uint64_t DOWN = 0x00000000ffffffff;

// stands for up_to_down
inline uint64_t utd(uint64_t x) {
    return (x & UP) >> 32;
}

#define SETWC(X, T) {       \
    subres = X + carry;     \
    T.push(subres & DOWN);  \
    carry = utd(subres);    \
}                           \

struct natural {
    std::vector<uint64_t> n = {};

    natural(int x = 0) {
        auto ux = static_cast<uint64_t>(x);
        n = {ux & DOWN};
        if (ux > DOWN)
            n.push_back((utd(x)));
    }

    natural(uint64_t x) {
        n = {x & DOWN};
        if (x > DOWN)
            n.push_back((utd(x)));
    }

    natural(double x) {
        n = {};
        x = std::floor(x);
        while (x) {
            push(std::fmod(x, std::pow(2, 32)));
            x /= std::pow(2, 32);
            x = std::floor(x);
        }
    }

    natural(std::vector<uint64_t> &&n) : n{n} {}

    void printn() const {
        std::cout << "nat: ";
        for (uint64_t x: n)
            std::cout << x << " ";
        std::cout << std::endl;

    }

    void push(uint64_t x) {
        n.push_back(x);
    }

    void pop() {
        n.pop_back();
    }

    uint64_t len() const {
        return n.size();
    }

    uint64_t &operator[](size_t i) {
        return n[i];
    }

    uint64_t operator[](size_t i) const {
        return n[i];
    }

    natural &operator=(uint64_t r) {
        (*this) = natural(r);
        return *this;
    }

    uint64_t to_ulong() const {
        uint64_t res = n[0];
        if (len() > 1)
            res += n[1] << 32;
        return res;
    }

    friend bool operator==(const natural &l, const natural &r) {
        if (l.len() != r.len())
            return false;
        for (size_t i = 0; i < l.len(); ++i) {
            if (l[i] != r[i])
                return false;
        }
        return true;
    }

    friend auto operator<=>(const natural &l, const natural &r) {
        if (auto cmp = l.len() <=> r.len(); cmp != 0)
            return cmp;
        for (size_t i = l.len() - 1; i > 0; --i) {
            if (auto cmp = l[i] <=> r[i]; cmp != 0)            
                return cmp;
        }
        return l[0] <=> r[0]; 
    }

    friend natural operator+(const natural &l, const natural &r) {
        uint64_t carry = 0, subres = 0;
        natural res = 0;
        res.n.clear();
        for (size_t i = 0; i < std::max(l.len(), r.len()); ++i) {
            uint64_t li = i < l.len() ? l[i] : 0;
            uint64_t ri = i < r.len() ? r[i] : 0;
            SETWC(li + ri, res);
        }
        if (carry)
            res.push(carry);

        return res;
    }

    natural &operator+=(const natural &r) {
        uint64_t carry = 0;
        natural res = 0;
        res.n.clear();
        size_t i = 0;
        for (; i < len(); ++i) {
            uint64_t ri = i < r.len() ? r[i] : 0;
            n[i] += ri + carry;
            carry = n[i] > DOWN;
            n[i] &= DOWN;
        }
        for (; i < r.len(); ++i) {
            push(r[i] + carry);
            carry = n.back() > DOWN;
            n.back() &= DOWN;
        }
        if (carry)
            push(carry);

        return *this;

    }

    natural operator-(const natural &r) const {
        uint64_t carry = 0, subres = 0;
        natural res = 0;
        res.n.clear();
        for (size_t i = 0; i < len(); ++i) {
            uint64_t ri = i < r.len() ? r[i] : 0;
            subres = (DOWN + 1 + n[i]) - (ri + carry);
            res.push(subres & DOWN);
            carry = n[i] < ri;
        }
        while (res.len() > 1 && !res.n.back())
            res.pop();
        return res;
    }

    natural &operator-=(const natural &r) {
        uint64_t carry = 0, subres = 0;
        for (size_t i = 0; i < len(); ++i) {
            uint64_t ri = i < r.len() ? r[i] : 0;
            subres = (DOWN + 1 + n[i]) - (ri + carry);
            carry = n[i] < ri;
            n[i] = (subres & DOWN);
        }
        while (!n.back() && len() > 1)
            pop();
        return *this;
    }

    natural &operator++() {
        uint64_t carry = 1;
        std::size_t i = 0;
        while (carry && i < len()) {
            ++n[i];
            carry = n[i] > DOWN;
            n[i] &= DOWN;
            ++i;
        }
        if (carry)
            push(1);
        return *this;
    }

    friend natural dac_sum(const std::vector<natural> &subs, size_t i, size_t j) {
        if (i == j)
            return subs[i];
        size_t mid = (i + j) / 2;
        natural l = dac_sum(subs, i, mid);
        natural r = dac_sum(subs, mid + 1, j);
        return l + r;
    }

    friend natural operator*(const natural &l, const natural &r) {
        uint64_t carry = 0;
        uint64_t subres = 0;
        std::vector<natural> subreses (l.len());
        subreses[0].pop();
        for (size_t i = 1; i < l.len(); ++i)
            for (size_t j = 0; j < i - 1; ++j)
                subreses[i].push(0);
        for (size_t i = 0; i < l.len(); ++i) {
            carry = 0;
            for (size_t j = 0; j < r.len(); ++j) {
                SETWC(l[i] * r[j], subreses[i]);
            }
            if (carry)
                subreses[i].push(carry);
        }
        carry = 0;
        natural res = dac_sum(subreses, 0, l.len() - 1);
        for (uint64_t &x: res.n) {
            x += carry;
            carry = utd(x);
            x &= DOWN;
        }
        while (res[res.len() - 1] == 0 && res.len() > 1)
            res.pop();

        return res;
    }


    natural power(int x) const {
        if (x == 0)
            return (1);
        natural res = power(x / 2);
        if (x % 2 == 0)
            return res * res;
        return (*this) * res * res;
    }

    std::pair<natural, natural> qr_division(const natural &r) const {
        natural quo = natural(0);
        natural rem = *this;
        while (rem >= r) {
            natural factor = 1;
            natural prev_fac = 0;
            natural prev_div = 1;
            natural div = r;
            while (rem >= div) {
                prev_div = div;
                div += div;
                prev_fac = factor;
                factor += factor;
            }
            rem -= prev_div;
            quo += prev_fac;
        }
        return {quo, rem};
    }

    natural operator/(const natural &r) const {
        return qr_division(r).first;
    }

    natural operator%(const natural &r) const {
        return qr_division(r).second;
    }

    std::vector<natural> digits(natural base) const {
        if (len() == 1 && n[0] == 0)
            return {{0}};
        natural aux = *this;
        std::vector<natural> res = {};
        while (aux > natural(0)) {
            auto p = aux.qr_division(base);
            aux = p.first;
            res.push_back(p.second);
        }
        std::reverse(res.begin(), res.end());
        return res;
    };

    double to_double() const {
        double res = n[0];
        for (std::size_t i = 1; i < len(); ++i)
            res += static_cast<double>(n[i]) * std::pow(std::pow(2.0, 32.0), i);

        return res;
    }

};

int main()
{
    natural m( 2.1 ), n( 2.9 );
    assert(m.len() && n.len());
    assert( m == n );
    assert( m / n == 1 );
    assert( m % n == 0 );
    assert( m.digits( 10 ).size() == 1 );
    assert( m.to_double() == 2.0 );
    natural d1 { std::pow( 2, 130 ) };
    //1.361129467683754e+39 
    natural d2 { 2 };
    double dist { d1.to_double() - std::pow( 2, 130 ) };
    // -1.361129467683754e+39 
    assert(std::fabs( dist ) <= std::pow( 2, 130 - 52 ));
    for (int i = 0; i < 130; ++i) {
        assert(d1 == natural(std::pow(2, 130 - i)));
        assert(d1 % d2 == 0);
        d1 = d1 / d2;
    }
    assert(d1 == 1);

    /*
    int f = 60;
    int b = 5;
    printf("%f\n", std::fmod(std::pow(b, f), b));
    natural d3 (std::pow(b, f));
    natural d4 (b);
    for (int i = 0; i < f; ++i) {
        //assert(d4 == natural(std::pow(4, x - i)));
        
        assert(d3 % d4 == 0);
        d3 = d3 / d4;
    }
    assert(d3 == 1);
    */
    natural base(2 * std::pow(1777, 3));
    n = natural();
    assert(n.digits(base).size() == 1);
    return 0;
}
