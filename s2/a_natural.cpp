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

void extend_vec(std::vector<bool> &t, const std::vector<bool> &s) {
    t.insert(t.end(), s.begin(), s.end());
}

struct natural;

natural dac_sum(const std::vector<natural> &, std::size_t, std::size_t);

struct natural {
    std::vector<bool> n = {};

    natural(int x = 0) {
        for (std::size_t i = 0; i < sizeof(int) - 1; ++i)
            n.emplace_back(x & (1 << i));
    }

    natural(double x) {
        auto ux = static_cast<uint64_t>(x);
        for (std::size_t i = 0; i < 64; ++i)
            n.emplace_back(ux & (1 << i));
    }

    natural(std::vector<bool> &&n) : n{n} {}

    std::size_t len() const {
        return n.size();
    }

    void pop() {
        n.pop_back();
    }

    void push(bool b) {
        n.push_back(b);
    }

    bool operator[](size_t i) const {
        return n[i];
    }

    void printn() const {
        uint64_t aux = 0;
        std::size_t j = 1;
        for (std::size_t i = 0; i < len(); ++i, j = (j % 64) + 1) {
            aux |= static_cast<uint64_t>(n[i]) << j;
            if (j == 64) {
                printf("%lu ", aux);
                aux = 0;
            }
        }
        if (j > 1)
            printf("%lu ", aux);
        printf("\n");
    }

    uint64_t to_u64() const {
        uint64_t res = 0;
        for (std::size_t i = 0; i < std::min<std::size_t>(64, len()); ++i)
            res |= static_cast<uint64_t>(n[i]) << i;
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
    
    natural operator+(const natural &r) const {
        std::vector<bool> res;
        bool carry = false;
        for (size_t i = 0; i < std::max(len(), r.len()); ++i) {
            bool li = i < len() ? n[i] : false;
            bool ri = i < r.len() ? r[i] : false;
            res.push_back(li ^ ri ^ carry);
            carry = (li && ri) || (ri && carry) || (carry && li);
        }
        if (carry)
            res.push_back(carry);
        return res;
    }

    natural operator-(const natural &r) const {
        std::vector<bool> res;
        bool carry = false;
        for (size_t i = 0; i < std::max(len(), r.len()); ++i) {
            bool li = i < len() ? n[i] : false;
            bool ri = i < r.len() ? r[i] : false;
            res.push_back(li - ri - carry);
            carry = (!li && (ri || carry)) || (li && ri && carry);
        }
        while (len() && !res.back())
            res.pop_back();

        return res;
    }


    natural operator*(const natural &r) const {
        std::vector<natural> subreses (len());
        subreses[0].pop();
        for (size_t i = 1; i < len(); ++i) {
            if (!r[i])
                continue;
            for (size_t j = 0; j < i - 1; ++j)
                subreses[i].push(false);
            extend_vec(subreses[i].n, n);

        }
        return dac_sum(subreses, 0, len() - 1);
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
        return {quo, rem};
    }

    uint64_t operator/(const natural &r) {
        return qr_division(r).first.to_u64();
    }

    uint64_t operator%(const natural &r) {
        return qr_division(r).second.to_u64();
    }

    std::vector<natural> digits(natural base) const {
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
        for (std::size_t i = 1; i < len(); ++i) {
            res += static_cast<double>(n[i]) * (32 << i);
        }
        return res;
    }

};

natural dac_sum(const std::vector<natural> &subs, size_t i, size_t j) {
    if (i == j)
        return subs[i];
    size_t mid = (i + j) / 2;
    natural l = dac_sum(subs, i, mid);
    natural r = dac_sum(subs, mid + 1, j);
    return l + r;
}

int main()
{
    natural m( 2.1 ), n( 2.9 );
    assert(m.len() && n.len());
    assert( m == n );
    assert( m / n == 1 );
    assert( m % n == 0 );
    assert( m.digits( 10 ).size() == 1 );
    assert( m.to_double() == 2.0 );

    return 0;
}
