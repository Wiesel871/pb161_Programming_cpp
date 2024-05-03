#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <numeric>
#include <queue>

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
const uint64_t BASE = 0x0000000100000000;

// stands for up_to_down
constexpr inline uint64_t utd(uint64_t x) {
    return (x & UP) >> 32;
}

struct carry {
    uint64_t val = 0;

    carry() = default;
    constexpr carry(uint64_t val) : val{val} {}

    constexpr void eval(uint64_t x, uint64_t &subres, std::vector<uint64_t> &t) {
        subres = x + val;
        t.push_back(subres & DOWN);
        val = utd(subres);
    }

    constexpr operator uint64_t() const {
        return val;
    }
};

std::vector<std::size_t> createRange(int start, int end) {
    std::vector<std::size_t> res(end - start + 1);
    std::iota(res.begin(), res.end(), start);
    return res;
}


struct natural {
    private:
    std::vector<uint64_t> n = {};

    public:

// constructors #CC (the #XX is for jumping around the file)
//-----------------------------------------------------------------------------

    constexpr natural(int x = 0) {
        auto ux = static_cast<uint64_t>(x);
        n = {ux & DOWN};
        if (ux > DOWN)
            n.push_back((utd(x)));
    }

    constexpr natural(uint64_t x) {
        n = {x & DOWN};
        if (x > DOWN)
            n.push_back((utd(x)));
    }

    constexpr natural(double x) {
        n = {};
        x = std::floor(x);
        while (x) {
            push(std::fmod(x, std::pow(2, 32)));
            x /= std::pow(2, 32);
            x = std::floor(x);
        }
    }

    constexpr natural(std::vector<uint64_t> &&n) : n{n} {}

    constexpr natural(natural &&r) {
        n = std::move(r.n);
    }

    constexpr natural(const natural &r) {
        n = r.n;
    }


// utility #UT
// ----------------------------------------------------------------------------
    
    
    constexpr void shiftR() {
        natural old = *this;
        uint64_t carry = 0;
        for (std::size_t i = 0; i < len(); ++i) {
            carry = (i < len() - 1 && n[i + 1] & 1) * (uint64_t{1} << 31);
            n[i] >>= 1;
            n[i] |= carry;
        }
        if (n[len() - 1] == 0)
            pop();
        assert(old == 0 || old != *this);
    }

    constexpr void shiftL() {
        natural old = *this;
        bool carry = false;
        for (std::size_t i = 0; i < len(); ++i) {
            n[i] <<= 1;
            n[i] += carry;
            carry = n[i] & BASE;
            n[i] ^= BASE;
        }
        if (carry)
            push(1);
        assert(old == 0 || old != *this);
    }

    constexpr natural copy() const {
        return *this;
    }

    friend void mergeQ(std::queue<natural> &q1, std::queue<natural> &q2) {
        while (!q2.empty()) {
            q1.push(std::move(q2.front()));
            q2.pop();
        }
    }

    constexpr void purgeZeroes() {
        while (!n.back() && len() > 1)
            pop();
    }

    constexpr friend natural baseTo(std::size_t l) {
        if (!l)
            return {1};
        std::vector<uint64_t> res = {};

        for (std::size_t i = 0; i < l - 1; ++i)
            res.push_back(0);
        res.push_back(1);
        return res;
    }

    std::pair<natural, natural> splittedAt(size_t i) const {
        if (i > len())
            return {natural(0), natural(*this)};
        return {
            natural(std::vector<uint64_t>(n.begin() + i, n.end())),
            natural(std::vector<uint64_t>(n.begin(), n.begin() + i))
        };
    }

    void printn() const {
        for (uint64_t x: n)
            std::cout << x << " ";
        std::cout << std::endl;

    }

    void printnb() const {
        for (uint64_t x: n) 
            for (size_t i = 0; i < 8; ++i)
                printf("%d", static_cast<bool>(x & (1 << i)));
        printf("\n");
    }

    constexpr void push(uint64_t x) {
        n.push_back(x);
    }

    constexpr void pop() {
        n.pop_back();
    }

    constexpr uint64_t len() const {
        return n.size();
    }

    constexpr uint64_t &operator[](size_t i) {
        return n[i];
    }

    constexpr uint64_t operator[](size_t i) const {
        return n[i];
    }

    constexpr uint64_t to_ulong() const {
        uint64_t res = n[0];
        if (len() > 1)
            res += n[1] << 32;
        return res;
    }


// assigners #AS
// ----------------------------------------------------------------------------
    constexpr natural &operator=(uint64_t r) {
        (*this) = natural(r);
        return *this;
    }

    constexpr natural &operator=(natural &&r) {
        n = std::move(r.n);
        return *this;
    }

    natural &operator=(const natural &r) = default;


// bool operators #BO
// ----------------------------------------------------------------------------
    constexpr bool operator==(const natural &r) {
        if (len() != r.len())
            return false;
        return std::equal(n.rbegin(), n.rend(), r.n.rbegin(), r.n.rend());
    }

    constexpr auto operator<=>(const natural &r) {
        if (auto cmp = len() <=> r.len(); cmp != 0)
            return cmp;
        return std::lexicographical_compare_three_way(n.rbegin(), n.rend(), r.n.rbegin(), r.n.rend());
    }

// arithmetic operators #AO
// ----------------------------------------------------------------------------
    constexpr natural operator+(const natural &r) {
        carry carry;
        uint64_t subres = 0;
        natural res = 0;
        res.n.clear();
        for (size_t i = 0; i < std::max(len(), r.len()); ++i) {
            uint64_t li = i < len() ? n[i] : 0;
            uint64_t ri = i < r.len() ? r[i] : 0;
            carry.eval(li + ri, subres, res.n);
        }
        if (carry)
            res.push(carry);

        return res;
    }

    constexpr natural operator-(const natural &r) const {
        uint64_t carry = 0, subres = 0;
        natural res = 0;
        res.n.clear();
        for (size_t i = 0; i < len(); ++i) {
            uint64_t ri = i < r.len() ? r[i] : 0;
            subres = (DOWN + 1 + n[i]) - (ri + carry);
            res.push(subres & DOWN);
            carry = n[i] < ri;
        }
        res.purgeZeroes();
        return res;
    }

    constexpr natural karatsuba_simple(const natural &r) const {
        assert(len() == 1);
        carry carry(0);
        natural res(0);
        res.n.clear();
        uint64_t subres = 0;
        for (size_t j = 0; j < r.len(); ++j) {
            carry.eval(n[0] * r[j], subres, res.n);
        }
        if (carry)
            res.push(carry);
        return res;
    }

    constexpr natural karatsuba(const natural &r) const {
        if (len() == 1) {
            if (n[0] == 0)
                return 0;
            if (n[0] == 1)
                return r;
            return karatsuba_simple(r);
        }
        if (r.len() == 1) {
            if (r[0] == 0)
                return 0;
            if (r[0] == 1)
                return *this;
            return r.karatsuba_simple(*this);
        }
        uint64_t m = std::max(len(), r.len());
        uint64_t m2 = m / 2;

        auto [h1, l1] = splittedAt(m2);
        auto [h2, l2] = r.splittedAt(m2);

        natural z0 = l1.karatsuba(l2);
        natural z1 = (l1 + h1).karatsuba(l2 + h2);
        natural z2 = h1.karatsuba(h2);

        natural res = (z2 << (m2 * 2)) + ((z1 - z2 - z0) << m2) + z0;
        res.purgeZeroes();
        return res;
    }

    constexpr natural operator*(const natural &r) const {
        return karatsuba(r);
    }

    constexpr natural operator<<(std::size_t r) const {
        if (!r)
            return *this;
        std::vector<uint64_t> res = {};
        for (std::size_t i = 0; i < r; ++i)
            res.push_back(0);
        res.insert(res.end(), n.begin(), n.end());
        return res;
    }

    constexpr natural operator>>(std::size_t r) const {
        if (!r)
            return *this;
        if (r >= len())
            return 0;
        return std::vector<uint64_t>(n.begin() + (len() - r), n.end());
    }

    constexpr std::pair<natural, natural> qr_division(const natural &r) const {
        natural rem = 0;
        natural quo = 0;
        quo.n.resize(len());
        for (size_t i = len(); i > 0; i--) {
            natural aux = natural(n[i - 1]) + rem * BASE;

            uint64_t lo = 0, ro = DOWN;
            uint64_t res = 0, check = 0;
            while (lo <= ro) {
                check = (lo + ro) >> 1;

                if (natural(check) * r <= aux) {
                    res = check; 
                    lo = check + 1;
                }
                else {
                    ro = check - 1;
                } 
            }

            quo[i - 1] = res;
            rem = aux - natural(res) * r;
        }

        quo.purgeZeroes();
        rem.purgeZeroes();
        return {std::move(quo), std::move(rem)};
    }

    /*
    std::pair<natural, natural> recDivRem(const natural &r) const {
        std::size_t m = len() - r.len();
        if (m < 2)
            return qr_division(r);
        std::size_t k = m / 2;
        auto [r1, r0] = r.splittedAt(k);
        auto [l1, l0] = splittedAt(2 * k);
        auto [Q1, R1] = l1.recDivRem(r1);
        natural aux = Q1 * (r0 << k) - l0 - R1 << (2 * k);
        natural rs = r << k;
        natural prev = 0;
        while (aux > natural(0)) {
            Q1 -= 1;
            prev = aux;
            aux -= rs;
        }
        aux = rs - prev;
        auto [a1, a0] = aux.splittedAt(k);
        auto [Q0, R0] = a1.recDivRem(r1);
        aux = Q0 * r0 - (R0 << k) - a0; 
        prev = 0;
        while (aux > 0) {
            Q0 -= 1;
            prev = aux;
            aux -= r;
        }
        aux = r - prev;
        return {(Q1 << k) + Q0, aux};
    }
    */

    constexpr natural operator/(const natural &r) const {
        return qr_division(r).first;
        //return recDivRem(r).first;
    }

    constexpr natural operator%(const natural &r) const {
        return qr_division(r).second;
        //return recDivRem(r).second;
    }


// arithmetic assigners #AA
// ----------------------------------------------------------------------------
    constexpr natural &operator+=(const natural &r) {
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

    constexpr natural &operator-=(const natural &r) {
        uint64_t carry = 0, subres = 0;
        for (size_t i = 0; i < len(); ++i) {
            uint64_t ri = i < r.len() ? r[i] : 0;
            subres = (DOWN + 1 + n[i]) - (ri + carry);
            carry = n[i] < ri;
            n[i] = (subres & DOWN);
        }
        purgeZeroes();
        return *this;
    }

    constexpr natural &operator++() {
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

// other #OT
// ----------------------------------------------------------------------------

    // n 3
    constexpr natural power(int n) const {
        natural x = *this;
        natural res(1);
        while (n) {
            if (n & 1) {
                res = res * x;
            }
            x = x * x;
            n >>= 1;
        }
        return res;
    }

    std::vector<natural> digits(double base) const {
        if (len() == 1 && n[0] == 0)
            return {{0}};
        return digits(natural(base));
    }

    std::vector<natural> digits(int base) const {
        if (len() == 1 && n[0] == 0)
            return {{0}};
        return digits(natural(base));
    }

    std::vector<natural> digits(const natural &base) const {
        return primitiveDigits(base);
        /*
        auto subres = copy().schohange_digits(base);
        std::vector<natural> res = {};
        while (!subres.empty()) {
            res.emplace_back(std::move(subres.front()));
            subres.pop();
        }
        return res;
        */
    };

    std::vector<natural> primitiveDigits(const natural &base) const {
        std::vector<natural> res = {};
        natural aux = *this;
        while (aux > natural(0)) {
            auto [q, r] = aux.qr_division(base);
            aux = std::move(q);
            res.emplace_back(std::move(r));
        }
        std::reverse(res.begin(), res.end());
        return res;
    }

    /*
    std::queue<natural> schohange_digits(natural base) {
        std::queue<natural> res = {};
        if (*this < base) {
            res.emplace(std::move(*this));
            return res;
        }
        auto range = createRange(0, 33 * (len() / base.len()));
        auto k = *std::lower_bound(range.begin(), range.end(), 0, [&](std::size_t l, std::size_t) {
                natural b2k2 = base.power(2 * l - 2);
                return b2k2 < *this && *this < b2k2 * base * base;
                });
        --k;
        assert(k);
        auto [Q, R] = qr_division(base.power(k));
        //auto [Q, R] = recDivRem(base.power(k));
        auto r = R.schohange_digits(base);
        res = Q.schohange_digits(base);
        for (std::size_t i = 0; i < k - r.size(); ++i)
            res.push(0);
        mergeQ(res, r);
        return res;
    }
    */

    constexpr double to_double() const {
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
    //assert( m.digits( 10 ).size() == 1 );
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
    printf("start\n");
    natural base(2 * std::pow(1777, 3));
    natural aux = 1;
    n = 0;
    const size_t s = 200;
    auto r = createRange(0, s);
    for (auto i: r) {
        n += aux * i;
        aux = aux * base;
    }
    auto digs = n.digits(base);
    assert(digs.size() == s + 1);
    for (auto i: r)
        assert(digs[i] == s - i);
    return 0;
}
