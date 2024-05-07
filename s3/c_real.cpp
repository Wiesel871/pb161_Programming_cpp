#include <cassert>
#include <algorithm>
#include <cassert>
#include <compare>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <ostream>
#include <vector>
#include <iostream>
#include <cmath>
#include <numeric>
#include <queue>
#include <concepts>


/* Předmětem této úlohy je naprogramovat typ ‹real›, který
 * reprezentuje reálné číslo s libovolnou přesností a rozsahem.
 *
 * Z hodnot:
 *
 *  • ‹a›, ‹b› typu ‹real›,
 *  • ‹k› typu ‹int›
 *
 * nechť je lze utvořit tyto výrazy, které mají vždy přesný
 * výsledek:
 *
 *  • ‹a + b›, ‹a - b›, ‹a * b›, ‹a / b›,
 *  • ‹a += b›, ‹a -= b›, ‹a *= b›, ‹a /= b›,
 *  • ‹a == b›, ‹a != b›, ‹a < b›, ‹a <= b›, ‹a > b›, ‹a >= b›,
 *  • ‹-a› – opačná hodnota,
 *  • ‹a.abs()› – absolutní hodnota,
 *  • ‹a.reciprocal()› – převrácená hodnota (není definováno pro 0),
 *  • ‹a.power( k )› – mocnina (včetně záporné).
 *
 * Výrazy, které nemají přesnou explicitní (číselnou) reprezentaci
 * jsou parametrizované požadovanou přesností ‹p› typu ‹real›:
 *
 *  • ‹a.sqrt( p )› – druhá odmocnina,
 *  • ‹a.exp( p )› – exponenciální funkce (se základem ⟦e⟧),
 *  • ‹a.log1p( p )› – přirozený logaritmus ⟦\ln(1 + a)⟧, kde
 *    ⟦a ∈ (-1, 1)⟧.
 *
 * Přesností se myslí absolutní hodnota rozdílu skutečné (přesné) a
 * reprezentované hodnoty. Pro aproximaci odmocnin je vhodné použít
 * Newtonovu-Raphsonovu metodu (viz ukázka z prvního týdne). Pro
 * aproximaci transcendentálních funkcí (exponenciála a logaritmus)
 * lze s výhodou použít příslušných mocninných řad. Nezapomeňte
 * ověřit, že řady v potřebné oblasti konvergují. Při určování
 * přesnosti (počtu členů, které je potřeba sečíst) si dejte pozor
 * na situace, kdy členy posloupnosti nejprve rostou a až poté se
 * začnou zmenšovat.
 *
 * Konečně je-li ‹d› hodnota typu ‹double›, nechť jsou přípustné
 * tyto konverze:
 *
 *  • ‹real x( d )›, ‹static_cast< real >( d )›,
 *
 * Poznámka: abyste se vyhnuli problémům s nejednoznačnými
 * konverzemi, je vhodné označit konverzní konstruktory a operátory
 * pro hodnoty typu ‹double› klíčovým slovem ‹explicit›. */
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

    void eval(uint64_t x, uint64_t &subres, std::vector<uint64_t> &t) {
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

struct whole;
struct natural;

template <typename T>
concept WholeView = requires(T t) {
    { t.is_neg() } -> std::same_as<bool>;
    { t.get_p() } -> std::convertible_to<const natural &>;
};


template <typename  T>
concept RealView = WholeView<T> && requires(T t) {
    { t.get_q() } -> std::convertible_to<const natural &>;
};

template <WholeView W>
struct abs_wview {
    W *r;

    abs_wview(W *r) : r{r} {};

    bool is_neg() const {
        return !r->is_neg();
    }

    const natural &get_p() const {
        return r->get_p();
    }

    whole &operator*() {
        return *r;
    }
};


template <RealView R>
struct abs_rview :  abs_wview<R> {
    abs_rview(R *r) : abs_wview<R>{r} {};

    const natural &get_q() const {
        return abs_wview<R>::r->get_q();
    }
};

template <WholeView W>
struct neg_wview {
    const W *r;

    neg_wview(W *r) : r{r} {};

    bool is_neg() const {
        return !r->is_neg();
    }

    const natural &get_p() const {
        return r->get_p();
    }
};

template <RealView R>
struct neg_rview :  neg_wview<R> {
    neg_rview(R *r) : neg_wview<R>{r} {};

    const natural &get_q() const {
        return neg_wview<R>::r->get_q();
    }
};

template <RealView R>
struct reci_view {
    const R *r;
    reci_view(R *r) : r{r} {}

    bool is_neg() const {
        return r->is_neg();
    }

    const natural &get_q() const {
        return r->get_p();
    }

    const natural &get_p() const {
        return r->get_q();
    }
};

struct natural {
    private:
    std::vector<uint64_t> n;

    public:
    bool is_neg() const {
        return false;
    }

    const natural &get_p() const {
        return *this;
    }

// constructors #CC (the #XX is for jumping around the file)
//-----------------------------------------------------------------------------

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

    natural(natural &&r) {
        n = std::move(r.n);
    }

    natural(const natural &r) {
        n = r.n;
    }


// utility #UT
// ----------------------------------------------------------------------------
    
    
    void shiftR() {
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

    void shiftL() {
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

    natural copy() const {
        return *this;
    }

    friend void mergeQ(std::queue<natural> &q1, std::queue<natural> &q2) {
        while (!q2.empty()) {
            q1.push(std::move(q2.front()));
            q2.pop();
        }
    }

    void purgeZeroes() {
        while (!n.back() && len() > 1)
            pop();
    }

    friend natural baseTo(std::size_t l) {
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
        assert(i < len());
        return n[i];
    }

    uint64_t operator[](size_t i) const {
        assert(i < len());
        return n[i];
    }

    uint64_t to_ulong() const {
        uint64_t res = n[0];
        if (len() > 1)
            res += n[1] << 32;
        return res;
    }


// assigners #AS
// ----------------------------------------------------------------------------
    natural &operator=(uint64_t r) {
        (*this) = natural(r);
        return *this;
    }

    natural &operator=(natural &&r) {
        n = std::move(r.n);
        return *this;
    }

    natural &operator=(const natural &r) = default;

    natural &operator=(whole &&);


// bool operators #BO
// ----------------------------------------------------------------------------
// beruc do uvahy ze algorithmy boli az tyzden po ukonceni tejto kapitoly
// si nemyslim ze to bolo najferovejsia kritika ale opravene

    bool operator==(const natural &r) const {
        if (len() != r.len())
            return false;
        return std::equal(n.rbegin(), n.rend(), r.n.rbegin(), r.n.rend());
    }

    auto operator<=>(const natural &r) const {
        if (auto cmp = len() <=> r.len(); cmp != 0)
            return cmp;
        return std::lexicographical_compare_three_way(n.rbegin(), n.rend(), r.n.rbegin(), r.n.rend());
    }

// arithmetic operators #AO
// ----------------------------------------------------------------------------
    natural operator+(const natural &r) const {
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

    natural plus(const natural &r) const {
        return *this + r;
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
        res.purgeZeroes();
        return res;
    }

    natural minus(const natural &r) const {
        return *this - r;
    }

    natural karatsuba_simple(const natural &r) const {
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

    natural karatsuba(const natural &r) const {
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

    natural operator*(const natural &r) const {
        return karatsuba(r);
    }

    natural operator<<(std::size_t r) const {
        if (!r)
            return *this;
        std::vector<uint64_t> res = {};
        for (std::size_t i = 0; i < r; ++i)
            res.push_back(0);
        res.insert(res.end(), n.begin(), n.end());
        return res;
    }

    natural operator>>(std::size_t r) const {
        if (!r)
            return *this;
        if (r >= len())
            return 0;
        return std::vector<uint64_t>(n.begin() + (len() - r), n.end());
    }

    std::pair<natural, natural> qr_division(const natural &r) const {
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

    natural operator/(const natural &r) const {
        return qr_division(r).first;
    }

    natural operator%(const natural &r) const {
        return qr_division(r).second;
    }


// arithmetic assigners #AA
// ----------------------------------------------------------------------------
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

    natural &operator-=(const natural &r) {
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

// other #OT
// ----------------------------------------------------------------------------

    natural power(int n) const {
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

    double to_double() const {
        double res = n[0];
        for (std::size_t i = 1; i < len(); ++i)
            res += static_cast<double>(n[i]) * std::pow(std::pow(2.0, 32.0), i);
        return res;
    }

    void printd() const {
        auto aux = digits(10);
        for (const auto &n: aux) {
            std::cout << n[0];
        }
        std::cout << std::endl;
    }
};

struct real;

struct whole {
    private:
    bool neg = false;
    natural p;
    friend real;

    public:
    const natural &get_p() const {
        return p;
    }
    natural &get_p() {
        return p;
    }

    bool is_neg() const {
        return neg;
    }


    whole() : p(0) {};
    whole(int i) : neg(i < 0), p(i) {};
    whole(double i) : neg(i < 0), p(i) {};

    whole(const natural &n) : p(n) {};
    whole(natural &&n) : p(n) {};

    whole &operator=(const natural &r) {
        p = r;
        return *this;
    }
    whole &operator=(natural &&r) {
        p = r;
        neg = false;
        return *this;
    }

    whole &operator=(int i) {
        p = i;
        neg = i < 0;
        return *this;
    }

    whole operator-() const {
        whole res = *this;
        res.neg = !neg;
        return res;
    }

    whole abs() const {
        whole res = *this;
        res.neg = false;
        return res;
    }

//---------------------------------------------------------------------------

    whole operator*(const auto &r) const {
        whole res;
        res.neg = neg != r.is_neg();
        res.p = p * r.p;
        return res;
    }

    whole &operator*=(const whole &r) {
        neg = neg != r.is_neg();
        p = p * r.p;
        return *this;
    }

    whole operator/(const whole &r) const {
        whole res;
        res.neg = neg != r.neg;
        res.p = p / r.p;
        return res;
    }

    whole &operator/=(const whole &r) {
        neg = neg != r.neg;
        p = p / r.p;
        return *this;
    }

//---------------------------------------------------------------------------
    whole operator*(const natural &r) const {
        whole res;
        res.neg = neg;
        res.p = p * r;
        return res;
    }

    whole &operator*=(const natural &r) {
        p = p * r;
        return *this;
    }

    whole operator/(const natural &r) const {
        whole res;
        res.neg = neg;
        res.p = p / r;
        return res;
    }

    whole &operator/=(const natural &r) {
        p = p / r;
        return *this;
    }

//---------------------------------------------------------------------------

    template <WholeView W>
    bool operator==(const W &r) const {
        return neg == r.is_neg() && p == r.get_p();
    }

    template <WholeView W>
    auto operator<=>(const W &r) const {
        if (neg && !r.is_neg())
            return std::strong_ordering::less;
        if (!neg && r.is_neg())
            return std::strong_ordering::greater;
        return p <=> r.get_p();
    }

//---------------------------------------------------------------------------

    template <WholeView W>
    whole plus(const W &r) const {
        whole res;
        res.neg = neg;
        res.p = p + r.get_p();
        return res;
    }

    template <WholeView W>
    whole minus(const W &r) const {
        whole res;
        res.neg = neg;
        res.p = p - r.get_p();
        return res;
    }

    template <WholeView W>
    whole operator+(const W &r) const {
        if (neg == r.is_neg())
            return plus(r);
        auto eq = p <=> r.get_p();
        if (eq == 0)
            return 0;
        if (eq < 0)
            return r.minus(*this);
        return minus(r);
    }

    template <WholeView W>
    whole operator-(const W &r) const {
        if (neg != r.is_neg())
            return plus(r);
        auto eq = p <=> r.get_p();
        if (eq == 0)
            return 0;
        if (eq < 0)
            return r.minus(*this);
        return minus(r);
    }

    template <WholeView W>
    whole &pluseq(const W &r) {
        p += r.get_p();
        return *this;
    }

    template <WholeView W>
    whole &minuseq(const W &r) {
        p -= r.get_p();
        return *this;
    }

    template <WholeView W>
    whole &operator+=(const W &r) {
        if (neg == r.is_neg())
            return pluseq(r);
        auto eq = p <=> r.get_p();
        if (eq == 0)
            return (*this = 0);
        if (eq < 0)
            return (*this = r.minus(get_p()));
        return minuseq(r);

    }

    template <WholeView W>
    whole &operator-=(const W &r) {
        if (neg != r.is_neg())
            return pluseq(r);
        auto eq = p <=> r.get_p();
        if (eq == 0)
            return (*this = 0);
        if (eq < 0)
            return (*this = r.minus(get_p()));
        return minuseq(r);

    }
//---------------------------------------------------------------------------

    whole power(int k) const {
        whole res;
        res.p = p.power(k);
        return res;
    }

    void print() const {
        std::cout << neg << std::endl;
        p.printd();
    }

    friend whole operator*(int i, const whole &r) {
        return whole(i) * r;
    }
    friend whole operator*(const whole &l, int i) {
        return l * whole(i);
    }

    friend whole operator/(int l, const whole &r) {
        return whole(l) / r;
    }

    friend whole operator/(const whole &l, int r) {
        return l / whole(r);
    }

    friend whole operator+(int l, const whole &r) {
        return whole(l) + r;
    }
    friend whole operator+(const whole &l, int r) {
        return l + whole(r);
    }

    friend whole operator-(int l, const whole &r) {
        return whole(l) - r;
    }
    friend whole operator-(const whole &l, int r) {
        return l - whole(r);
    }

    friend whole operator*(double i, const whole &r) {
        return whole(i) * r;
    }
    friend whole operator*(const whole &l, double i) {
        return l * whole(i);
    }

    friend whole operator/(double l, const whole &r) {
        return whole(l) / r;
    }

    friend whole operator/(const whole &l, double r) {
        return l / whole(r);
    }

    friend whole operator+(double l, const whole &r) {
        return whole(l) + r;
    }
    friend whole operator+(const whole &l, double r) {
        return l + whole(r);
    }

    friend whole operator-(double l, const whole &r) {
        return whole(l) - r;
    }
    friend whole operator-(const whole &l, double r) {
        return l - whole(r);
    }
    friend whole &operator+=(whole &l, int r) {
        return l += whole(r);
    }
    friend whole &operator-=(whole &l, int r) {
        return l -= whole(r);
    }
    friend whole &operator*=(whole &l, int r) {
        return l *= whole(r);
    }
    friend whole &operator/=(whole &l, int r) {
        return l += whole(r);
    }
    friend whole &operator+=(whole &l, double r) {
        return l += whole(r);
    }
    friend whole &operator-=(whole &l, double r) {
        return l -= whole(r);
    }
    friend whole &operator*=(whole &l, double r) {
        return l *= whole(r);
    }
    friend whole &operator/=(whole &l, double r) {
        return l += whole(r);
    }
};

natural &natural::operator=(whole &&w) {
    n = std::move(w.get_p().n);
    return *this;
}

struct real {
    private:
    whole p;
    natural q;

    public:
    const natural &get_p() const {
        return p.get_p();
    }

    const natural &get_q() const {
        return q;
    }

    bool is_neg() const {
        return p.is_neg();
    }

    real() : p(0), q(1) {}

    real(int i) : p(i), q(1) {}

    real(real &&r) = default;
    real(const real &r) = default;

    real &operator=(int i) {
        q = 1;
        p = i;
        return *this;
    }

    real &operator=(real &&r) = default;

    real &operator=(const real &r) = default;

    static natural gcd(natural a, natural b) {
        natural aux;
        while (a != 0) {
            aux = std::move(a);
            a = b % aux;
            b = std::move(aux);
        }
        return b;
    }

    void normalise() {
        natural g = gcd(p.get_p(), q);
        while (g > 1) {
            p = p / g;
            q = q / g;
            g = gcd(p.get_p(), q);
        }
    }

    real(double i) : p(i), q(1) {
        double fractional, whole;
        fractional = 10 * std::modf(i, &whole);
        whole = 0;
        natural ten = 10;
        while (fractional != 0.0) {
            fractional = std::modf(fractional, &whole);
            q = q * ten;
            p = p * ten;
            p += whole;
            fractional *= 10;
        }
        normalise();
    }

    real operator-() const {
        real res = *this;
        res.p.neg = !p.is_neg();
        return res;
    }

    real abs() const {
        real res;
        res.p = p.abs();
        res.q = q;
        return res;
    }

    real reciprocal() const {
        real res;
        res.p = q;
        res.p.neg = p.is_neg();
        res.q = p.get_p();
        return res;
    }

    template <RealView L, RealView R>
    friend real operator+(const L &l, const R &r) {
        real res;
        res.p = l.get_p() * r.get_q() + r.get_p() * l.get_q();
        res.q = l.get_q() * r.get_q();
        res.normalise();
        return res;
    } 

    template<RealView R>
    real &operator+=(const R &r) {
        p = p * r.get_q();
        p += r.get_p() * q;
        q = q * r.get_q();
        normalise();
        return *this;
    }

    template <RealView L, RealView R>
    friend real operator-(const L &l, const R &r) {
        real res;
        res.p = l.get_p() * r.get_q() - r.get_p() * l.get_q();
        res.q = l.get_q() * r.get_q();
        res.normalise();
        return res;
    } 

    template<RealView R>
    real &operator-=(const R &r) {
        p = p * r.get_q();
        p -= r.get_p() * q;
        q = q * r.get_q();
        normalise();
        return *this;
    }

    template <RealView L, RealView R>
    friend real operator*(const L &l, const R &r) {
        real res;
        res.p = l.get_p() * r.get_p();
        res.q = l.get_q() * r.get_q();
        res.normalise();
        return res;
    }

    template<RealView R>
    real &operator*=(const R &r) {
        p = p * r.get_p();
        q = q * r.get_q();
        normalise();
        return *this;
    }

    template <RealView L, RealView R>
    friend real operator/(const L &l, const R &r) {
        real res;
        res.p = l.get_p() * r.get_q();
        res.q = r.get_p() * l.get_q();
        res.normalise();
        return res;
    }

    template<RealView R>
    real &operator/=(const R &r) {
        p = p * r.get_q();
        q = r.get_p() * q;
        normalise();
        return *this;
    }

    template<RealView R>
    bool operator==(const R &r) const {
        return p == r.get_p() && q == r.get_q();
    }

    template<RealView R>
    auto operator<=>(const R &r) const {
        whole ln = p * r.get_q();
        whole rn = r.get_p() * q;
        return ln <=> rn;
    }

    real power(int k) const {
        if (!k)
            return 1;
        real res;
        res.p = p.power(std::abs(k));
        res.q = q.power(std::abs(k));
        if (k < 0) {
            whole aux = std::move(res.p);
            res.p = std::move(res.q);
            res.q = std::move(aux);
        }
        res.normalise();
        return res;
    }

    template<RealView R>
    real sqrt(const R &) const;

    template<RealView R>
    real exp(const R &p) const;

    template<RealView R>
    real log1p(const R &p) const;


    void print() const {
        std::cout << p.is_neg() << std::endl;
        p.print();
        q.printd();
        std::cout << "--------------------" << std::endl;
    }

    friend real operator*(int i, const real &r) {
        return real(i) * r;
    }
    friend real operator*(const real &l, int i) {
        return l * real(i);
    }

    friend real operator/(int l, const real &r) {
        return real(l) / r;
    }

    friend real operator/(const real &l, int r) {
        return l / real(r);
    }

    friend real operator+(int l, const real &r) {
        return real(l) + r;
    }
    friend real operator+(const real &l, int r) {
        return l + real(r);
    }

    friend real operator-(int l, const real &r) {
        return real(l) - r;
    }
    friend real operator-(const real &l, int r) {
        return l - real(r);
    }

    friend real operator*(double i, const real &r) {
        return real(i) * r;
    }
    friend real operator*(const real &l, double i) {
        return l * real(i);
    }

    friend real operator/(double l, const real &r) {
        return real(l) / r;
    }

    friend real operator/(const real &l, double r) {
        return l / real(r);
    }

    friend real operator+(double l, const real &r) {
        return real(l) + r;
    }
    friend real operator+(const real &l, double r) {
        return l + real(r);
    }

    friend real operator-(double l, const real &r) {
        return real(l) - r;
    }
    friend real operator-(const real &l, double r) {
        return l - real(r);
    }
    friend real &operator+=(real &l, int r) {
        return l += real(r);
    }
    friend real &operator-=(real &l, int r) {
        return l -= real(r);
    }
    friend real &operator*=(real &l, int r) {
        return l *= real(r);
    }
    friend real &operator/=(real &l, int r) {
        return l += real(r);
    }
    friend real &operator+=(real &l, double r) {
        return l += real(r);
    }
    friend real &operator-=(real &l, double r) {
        return l -= real(r);
    }
    friend real &operator*=(real &l, double r) {
        return l *= real(r);
    }
    friend real &operator/=(real &l, double r) {
        return l += real(r);
    }
    friend bool operator==(const real &l, int r) {
        return l == real(r);
    }
    friend bool operator==(int l, const real &r) {
        return real(l) == r;
    }
    friend auto operator<=>(const real &l, int i) {
        return l <=> real(i);
    }
    friend auto operator<=>(int l, const real &r) {
        return real(l) <=> r;
    }
    friend bool operator==(const real &l, double r) {
        return l == real(r);
    }
    friend bool operator==(double l, const real &r) {
        return real(l) == r;
    }
    friend auto operator<=>(const real &l, double i) {
        return l <=> real(i);
    }
    friend auto operator<=>(double l, const real &r) {
        return real(l) <=> r;
    }
};

abs_rview<const real> to_abs_view(const real *r) {
    return {r};
};

abs_rview<const real> to_neg_view(const real *r) {
    return {r};
};

template<RealView R>
real real::sqrt(const R &p) const {
    real res = *this;

    real aux = res * res - *this;
    while (to_abs_view(&aux) > p) {
        res = (res + *this / res) / 2; 
        aux = res * res - *this;
    }
    res.normalise();
    return res;
}

template<RealView R>
real real::exp(const R &p) const {
    real res(1.0);

    real term(1.0);
    real n = 1;
    while (to_abs_view(&term) > p) {
        term *= *this / n;
        res += term;
        n += 1;
    }
    res.normalise();
    return res;
}

template<RealView R>
real real::log1p(const R &p) const {
    real res = *this; 
    auto negative = to_neg_view(this);
    real term = *this;
    real n = 2;
    while (to_abs_view(&term) > p) {
        term *= negative * (n - 1) / n;
        res += term;
        n += 1;
    }

    res.normalise();
    return res;
}


int main()
{
    real zero = 0;
    std::cout << "zero" << std::endl;
    zero.print();
    real one = 1;
    std::cout << "one" << std::endl;
    one.print();
    real ten = 10;
    std::cout << "ten" << std::endl;
    ten.print();
    real half = one / 2;
    std::cout << "half" << std::endl;
    half.print();
    real three = one + 2;
    three -= 1;

    real eps = ten.power( -3 );
    std::cout << "eps" << std::endl;
    eps.print();

    assert(ten > one);

    real pi( 3.14159265 );
    std::cout << "pi" << std::endl;
    pi.print();
    real sqrt2( 1.41421356 );
    std::cout << "sqrt" << std::endl;
    sqrt2.print();
    real e( 2.71828182 );
    std::cout << "e" << std::endl;
    e.print();
    real l_half( 0.40546511 );
    std::cout << "l_half" << std::endl;
    l_half.print();

    assert( static_cast< real >( 1.0 ) == one );

    std::cout << "1 + -1" << std::endl;
    (one + -one).print();
    assert( one + -one == zero );

    std::cout << "1 * 10" << std::endl;
    (one * ten).print();
    assert( one * ten == ten );

    std::cout << "one exp" << std::endl;
    (one.exp(eps) - e).abs().print();
    assert( ( one.exp( eps ) - e ).abs() < eps );

    std::cout << "log1p" << std::endl;
    zero.log1p(eps).abs().print();
    assert( zero.log1p( eps ).abs() < eps );

    std::cout << "h log1p" << std::endl;
    (half.log1p(eps) - l_half).abs().print();
    assert( ( half.log1p( eps ) - l_half ).abs() < eps );

    real two(2.0);
    assert( ( ( one / two ).exp( eps ) - e.sqrt( eps ) ).abs() < 2 * eps );
    return 0;
}
