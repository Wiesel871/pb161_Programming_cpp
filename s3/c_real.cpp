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

template <typename T>
concept I64 = requires (T t) {
    { t } -> std::convertible_to<int64_t>;
};

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
struct real;

template <typename T>
concept WholeView = requires(T t) {
    { t.get_n() } -> std::same_as<const natural &>;
    { t.is_neg() } -> std::same_as<bool>;
};


template <typename  T>
concept RealView = WholeView<T> && requires(T t) {
    { t.get_p() } -> std::convertible_to<const whole &>;
    { t.get_q() } -> std::same_as<const natural &>;
};

template <RealView R>
struct reci_view {
    const R *r;
    reci_view(const R *r) : r{r} {}

    bool is_neg() const {
        return r->is_neg();
    }

    const natural &get_q() const {
        return r->get_n();
    }

    const natural &get_n() const {
        return r->get_q();
    }

    const whole &get_p() const {
        return r->get_q();
    }

    operator real() const;
};


template <WholeView W>
struct abs_wview {
    const W *r;

    abs_wview(const W *r) : r{r} {};

    bool is_neg() const {
        return false;
    }

    const natural &get_n() const {
        return r->get_n();
    }

    operator whole() const;
};


template <RealView R>
struct abs_rview {
    const abs_wview<R> in;
    const R *r;

    abs_rview(const R *r) : in{r}, r{r} {};

    const natural &get_n() const {
        return in.get_n();
    }

    bool is_neg() const {
        return false;
    }

    const natural &get_q() const {
        return r->get_q();
    }

    const abs_wview<R> &get_p() const {
        return in;
    }

    operator real() const;

    void print() const;

    abs_rview<R> abs() const {
        return this;
    }

    reci_view<R> reciprocal() const {
        return this;
    }

    real power(int k) const;

    template <RealView L>
    real sqrt(const L &p) const;

    template <RealView L>
    real exp(const L &p) const;
    
    template <RealView L>
    real log1p(const L &p) const;
};

template <WholeView W>
struct neg_wview {
    const W *r;

    neg_wview(const W *r) : r{r} {};

    bool is_neg() const {
        return !r->is_neg();
    }

    const natural &get_n() const {
        return r->get_n();
    }

    operator whole() const;
};

template <RealView R>
struct neg_rview {
    const neg_wview<R> in;
    const R *r;

    neg_rview(const R *r) : in{r}, r{r} {};

    const natural &get_n() const {
        return in.get_n();
    }

    bool is_neg() const {
        return in.is_neg();
    }

    const natural &get_q() const {
        return r->get_q();
    }

    const neg_wview<R> &get_p() const {
        return in;
    }

    operator real() const;

    void print() const;

    abs_rview<R> abs() const {
        return *this;
    }

    reci_view<R> reciprocal() const {
        return *this;
    }

    real power(int k) const;

    template <RealView L>
    real sqrt(const L &p) const;

    template <RealView L>
    real exp(const L &p) const;
    
    template <RealView L>
    real log1p(const L &p) const;
};

struct natural {
    private:
    std::vector<uint64_t> n;

    public:
    friend whole;
    friend real;

    const natural &get_n() const {
        return *this;
    }

    bool is_neg() const {
        return false;
    }

// constructors #CC (the #XX is for jumping around the file)
//-----------------------------------------------------------------------------

    template<I64 I>
    natural(I x) {
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

    natural &operator=(whole &&);

    natural &operator=(const natural &r) = default;


// bool operators #BO
// ----------------------------------------------------------------------------
    
    bool is_zero() const {
        return len() == 1 && n[0] == 0;
    }

    bool operator==(uint64_t i) const {
        return len() == 1 && n[0] == i;
    }

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

    template <I64 I>
    friend natural operator*(I i, const natural &r) {
        carry carry(0);
        natural res(0);
        res.n.clear();
        uint64_t subres = 0;
        for (size_t j = 0; j < r.len(); ++j) {
            carry.eval(i * r[j], subres, res.n);
        }
        if (carry)
            res.push(carry);
        return res;

    }

    template <I64 I>
    natural operator*=(I i) {
        uint64_t carry = 0;
        uint64_t subres = 0;
        for (size_t j = 0; j < len(); ++j) {
            subres = n[j] * i + carry;
            n[j] = subres & DOWN;
            carry = utd(subres);
            
        }
        if (carry)
            push(carry);
        return *this;
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

    template <I64 I>
    natural &operator+=(I r) {
        uint64_t carry = 0;
        natural res = 0;
        res.n.clear();
        uint64_t subres = n[0] + r;
        n[0] = subres & DOWN;
        carry = subres > DOWN;
        for (size_t i = 1; i < len(); ++i) {
            subres = n[i] + carry;
            n[i] = subres & DOWN;
            carry = subres > DOWN;
            
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


struct whole {
    bool neg = false;
    natural p;

    const natural &get_n() const {
        return p;
    }

    bool is_neg() const {
        return neg;
    }

    whole(int i = 0) : neg(i < 0), p(i) {}
    whole(int64_t i) : neg(i < 0), p(i) {}
    whole(std::size_t i) : p(i) {}
    whole(double i) : neg(i < 0), p(i) {}

    whole(const whole &w) = default;
    whole(whole &&w) : neg(w.neg), p(std::move(w.p)) {}

    whole(natural &&n) : p(n) {};

    whole &operator=(const whole &w) = default;
    whole &operator=(whole &&w) {
        neg = w.neg;
        p = std::move(w.p);
        return *this;
    }

    whole &operator=(const natural &r) {
        p = r;
        return *this;
    }
    whole &operator=(natural &&r) {
        p = std::move(r);
        neg = false;
        return *this;
    }
    whole &operator=(int i) {
        p = i;
        neg = i < 0;
        return *this;
    }

    void swap(whole &r) {
        whole aux(std::move(*this));
        *this = std::move(r);
        r = std::move(aux);
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

    template<WholeView W>
    whole &operator+=(const W &r) {
        if (!is_neg() && !r.is_neg())
            return *this = std::move(merge1(abs_wview<whole>(this), abs_wview<W>(&r)));
        if (!is_neg() && r.is_neg()) 
            return *this = std::move(merge2(abs_wview<whole>(this), abs_wview<W>(&r)));
        if (is_neg() && !r.is_neg())
            *this = std::move(merge2(abs_wview<whole>(this), abs_wview<W>(&r)));
        if (is_neg() && r.is_neg()) 
            *this = std::move(merge1(abs_wview<whole>(this), abs_wview<W>(&r)));
        neg = !neg;
        return *this;
    }


    template<WholeView W>
    whole &operator-=(const W &r) {
        if (!is_neg() && !r.is_neg())
            return *this = std::move(merge2(abs_wview<whole>(this), abs_wview<W>(&r)));
        if (!is_neg() && r.is_neg()) 
            return *this = std::move(merge1(abs_wview<whole>(this), abs_wview<W>(&r)));
        if (is_neg() && !r.is_neg())
            *this = std::move(merge1(abs_wview<whole>(this), abs_wview<W>(&r)));
        if (is_neg() && r.is_neg()) 
            *this = std::move(merge2(abs_wview<whole>(this), abs_wview<W>(&r)));
        neg = !neg;
        return *this;
    }

    template<WholeView W>
    whole &operator*=(const W &r) {
        neg = neg != r.is_neg();
        p = p * r.get_n();
        return *this;
    }

    template<WholeView W>
    whole &operator/=(const W &r) {
        neg = neg != r.is_neg();
        p = p / r.get_n();
        return *this;
    }

    whole &operator*=(int i) {
        p *= i;
        neg = neg != (i < 0);
        return *this;
    }


    whole power(int k) const {
        whole res;
        res.p = p.power(k);
        return res;
    }

    void print() const {
        std::cout << neg << std::endl;
        p.printd();
    }
};

natural &natural::operator=(whole &&w) {
    n = std::move(w.p.n);
    return *this;
}


template<WholeView W, WholeView V>
whole multw(const W &l, const V &r) {
    whole res;
    res.neg = l.is_neg() != r.is_neg();
    res.p = l.get_n() * r.get_n();
    return res;
}

template<WholeView W, WholeView V>
whole divw(const W &l, const V &r) {
    whole res;
    res.neg = l.is_neg() != r.is_neg();
    res.p = l.get_n() / r.get_n();
    return res;
}

template<WholeView W, WholeView V>
bool eqw(const W &l, const V &r) {
    return l.is_neg() == r.is_neg() && l.get_n() == r.get_n();
}

template<WholeView W, WholeView V>
auto compw(const W &l, const V &r) {
    if (l.is_neg() && !r.is_neg())
        return std::strong_ordering::less;
    if (!l.is_neg() && r.is_neg())
        return std::strong_ordering::greater;   
    if (l.is_neg() && r.is_neg())
        return 0 <=> (l.get_n() <=> r.get_n());
    return l.get_n() <=> r.get_n();
}



template<WholeView W, WholeView V>
whole merge1(const W &l, const V &r) {
    return l.get_n() + r.get_n();
}

template<WholeView W, WholeView V>
whole merge2(const W &l, const V &r) {
    auto eq = l.get_n() <=> r.get_n();
    if (eq == 0)
        return 0;
    if (eq < 0) {
        whole res;
        res = r.get_n() - l.get_n();
        res.neg = true;
        return res;
    }
    return l.get_n() - r.get_n();
}

template<WholeView W, WholeView V>
whole addw(const W &l, const V &r) {
    if (!l.is_neg() && !r.is_neg()) 
        return merge1(abs_wview<W>(&l), abs_wview<V>(&r));
    if (!l.is_neg() && r.is_neg()) 
        return merge2(abs_wview<W>(&l), abs_wview<V>(&r));
    whole res;
    if (l.is_neg() && !r.is_neg())
        res = std::move(merge2(abs_wview<W>(&l), abs_wview<V>(&r)));
    if (l.is_neg() && r.is_neg()) 
        res = std::move(merge1(abs_wview<W>(&l), abs_wview<V>(&r)));
    res.neg = !res.neg;
    return res;
}

template<WholeView W, WholeView V>
whole subw(const W &l, const V &r) {
    if (!l.is_neg() && !r.is_neg())
        return merge2(abs_wview<W>(&l), abs_wview<V>(&r));
    if (!l.is_neg() && r.is_neg()) 
        return merge1(abs_wview<W>(&l), abs_wview<V>(&r));
    whole res;
    if (l.is_neg() && !r.is_neg())
        res = std::move(merge1(abs_wview<W>(&l), abs_wview<V>(&r)));
    if (l.is_neg() && r.is_neg()) 
        res = std::move(merge2(abs_wview<W>(&l), abs_wview<V>(&r)));
    res.neg = !res.neg;
    return res;
}

struct real {
    whole p;
    natural q;
    static bool final;

    const natural &get_n() const {
        return p.get_n();
    }

    const whole &get_p() const {
        return p;
    }

    const natural &get_q() const {
        return q;
    }
    
    bool is_neg() const {
        return p.is_neg();
    }

    real() : p(0), q(1) {}

    real(std::size_t i) : p(i), q(1) {}

    real(int64_t i) : p(i), q(1) {}
    real(int i) : p(i), q(1) {}

    real(real &&r) = default;
    real(const real &) = default;

    template<RealView R>
    real(const R &r) : p{r.get_p()}, q{r.get_q()} {};

    real &operator=(std::size_t i) {
        q = 1;
        p = i;
        return *this;
    }

    real &operator=(int i) {
        q = 1;
        p = i;
        return *this;
    }

    real &operator=(real &&r) {
        p = std::move(r.p);
        q = std::move(r.q);
        return *this;
    };

    real &operator=(const real &r) {
        p.p = r.get_n();
        p.neg = r.is_neg();
        q = r.get_q();
        return *this;
    }

    template<RealView R>
    real &operator=(const R &r) {
        p.p = r.get_n();
        p.neg = r.is_neg();
        q = r.get_q();
        return *this;
    }

    static natural gcd(natural a, natural b) {
        natural aux = 0;
        while (a != 0) {
            aux = std::move(a);
            a = b % aux;
            b = std::move(aux);
        }
        return b;
    }

    void normalise() {
        if (p.p == 0) {
            q = 1;
            return;
        }
        natural g = gcd(p.p, q);
        p /= g;
        q = q / g;
    }

    explicit real(double i) : p(i), q(1) {
        double fractional, whole, aux = 0;
        fractional = 10 * std::modf(i, &whole);
        whole = 0;
        std::uint64_t mult = 1;
        while (fractional != 0.0) {
            if (mult > DOWN / 10) {
                q *= mult;
                p *= mult;
                p += real(static_cast<int>(aux));
                mult = 1;
                aux = 0;
            } else {
                fractional = std::modf(fractional, &whole);
                fractional *= 10;
                aux *= 10;
                mult *= 10;
                aux += whole;
            }
        }
        if (aux) {
            q *= mult;
            p *= mult;
            p += real(static_cast<int>(aux));
        }
        normalise();
    }

    template <RealView R>
    friend neg_rview<R> operator-(const R &r) {
        return &r;
    }

    template <RealView R>
    friend abs_rview<R> abs_view(const R &r) {
        return &r;
    }

    template <RealView R>
    friend reci_view<R> rec_view(const R &r) {
        return &r;
    }

    auto abs() const;

    real reciprocal() const {
        real res;
        res.p = q;
        res.p.neg = p.neg;
        res.q = p.p;
        return res;
    }

    template<RealView L, RealView R>
    friend real operator+(const L &l, const R &r) {
        real res;
        res.p = addw(multw(l.get_p(), r.get_q()), multw(r.get_p(), l.get_q()));
        res.q = multw(l.get_q(), r.get_q());
        res.normalise();
        return res;
    } 

    template<RealView R>
    real &operator+=(const R &r) {
        p *= r.get_q();
        p += multw(r.get_p(), q);
        q = multw(q, r.get_q());
        normalise();
        return *this;
    }

    template<RealView L, RealView R>
    friend real operator-(const L &l, const R &r) {
        real res;
        res.p = subw(multw(l.get_p(), r.get_q()), multw(r.get_p(), l.get_q()));
        res.q = multw(l.get_q(), r.get_q());
        res.normalise();
        return res;
    } 

    template<RealView R>
    real &operator-=(const R &r) {
        p *= r.get_q();
        p -= multw(r.get_p(), q);
        q = multw(q, r.get_q());
        normalise();
        return *this;
    }

    template<RealView L, RealView R>
    friend real operator*(const L &l, const R &r) {
        real res;
        res.p = multw(l.get_p(), r.get_p());
        res.q = multw(l.get_q(), r.get_q());
        res.normalise();
        return res;
    }

    template<RealView R>
    real &operator*=(const R &r) {
        p = multw(p, r.get_p());
        q = multw(q, r.get_q());
        normalise();
        return *this;
    }

    template<RealView L, RealView R>
    friend real operator/(const L &l, const R &r) {
        real res;
        res.p = multw(l.get_p(), r.get_q());
        res.q = multw(l.get_q(), r.get_p());
        res.normalise();
        return res;
    }

    template<RealView R>
    real &operator/=(const R &r) {
        p = multw(p, r.get_q());
        q = multw(q, r.get_p());
        normalise();
        return *this;
    }

    template<RealView L>
    friend auto operator==(const L &l, int64_t i) {
        return l.get_q() == 1 && l.get_p() == i;
    }

    template<RealView L>
    friend bool operator==(int64_t i, const L &l) {
        return l.get_q() == 1 && l.get_p() == i;
    }


    template<RealView L, RealView R>
    friend bool operator==(const L &l, const R &r) {
        return eqw(l.get_p(), r.get_p()) && l.get_q() == r.get_q();
    }

    template<RealView L, RealView R>
    friend auto operator<=>(const L &l, const R &r) {
        if (l.is_neg() != r.is_neg()) {
            if (l.is_neg())
                return std::strong_ordering::less;
            return std::strong_ordering::greater;
        }
        whole ln = multw(l.get_p(), r.get_q());
        whole rn = multw(r.get_p(), l.get_q());
        return compw(ln, rn);
    }

    friend real mov_abs(real &&r) {
        r.p.neg = false;
        return r;
    }


    template<RealView R>
    real exp(const R &) const;

    real power(int) const;

    template<RealView R>
    real sqrt(const R &) const;

    template<RealView R>
    real log1p(const R &) const;

    void print() const;
};
// vsetky if-i su len na prevenciu nadbytocnej alokacie
template<I64 I, RealView R>
bool operator==(I i, const R &r) {
    return r.get_q() == 1 && r.is_neg() == i < 0 && r.get_n() == i;
}

template<I64 I, RealView R>
bool operator==(const R &r, I i) {
    return r.get_q() == 1 && r.is_neg() == i < 0 && r.get_n() == i;
}

template<RealView R>
bool operator==(double d, const R &r) {
    return r == real(d);
}

template<RealView R>
bool operator==(const R &r, double d) {
    return r == real(d);
}

template<I64 I, RealView R>
auto operator<=>(I i, const R &r) {
    return r <=> real(i);
}

template<I64 I, RealView R>
auto operator<=>(const R &r, I i) {
    return r <=> real(i);
}

template<RealView R>
auto operator<=>(double d, const R &r) {
    return r <=> real(d);
}

template<RealView R>
auto operator<=>(const R &r, double d) {
    return r <=> real(d);
}

template<I64 I, RealView R>
real operator*(I i, const R &r) {
    if (!i || r.get_n() == 0) {
        return 0;
    }
    if (i == 1) 
        return r;
    if (i == -1) 
        return -r;
    real res = r;
    res.p *= i;
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator*(const R &r, I i) {
    if (!i || r.get_n() == 0) {
        return 0;
    }
    if (i == 1) 
        return r;
    if (i == -1) 
        return -r;
    real res = r;
    res.p *= i;
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator/(I i, const R &r) {
    if (!i) {
        return 0;
    }
    real res = r;
    res.q *= i;
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator/(const R &r, I i) {
    if (r.get_n() == 0) {
        return 0;
    }
    if (i == 1) 
        return r;
    if (i == -1) 
        return -r;

    real res = r;
    res.q *= i;
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator+(I i, const R &r) {
    if (!i) {
        return r;
    }
    real res = r;
    res.p += i * r.get_q();
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator+(const R &r, I i) {
    if (!i) {
        return r;
    }
    real res = r;
    res.p += i * r.get_q();
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator-(I i, const R &r) {
    if (!i) {
        return r;
    }
    real res = r;
    res.p -= i * r.get_q();
    res.normalise();
    return res;
}

template<I64 I, RealView R>
real operator-(const R &r, I i) {
    if (!i) {
        return r;
    }
    real res = r;
    res.p -= i * r.get_q();
    res.normalise();
    return res;
}

template<RealView R>
real operator*(double i, const R &r) {
    if (!i || r.get_p() == 0) {
        return 0;
    }
    return r * real(i);
}

template<RealView R>
real operator/(double i, const R &r) {
    if (!i || r.get_n() == 0) {
        return 0;
    }
    return real(i) / r;
}

template<RealView R>
real operator+(double i, const R &r) {
    if (!i)
        return r;
    return r + real(i);
}

template<RealView R>
real operator-(double i, const R &r) {
    if (!i)
        return r;
    return real(i) - r;
}

template<RealView R, RealView L>
real gexp(const L &l, const R &r) {
    real res(1.0);
    real term(1.0);
    abs_rview<real> abs_term(&term);
    int n = 1;
    while (abs_term > r) {
        term *= l / n;
        res += term;
        n++;
    }
    res.normalise();
    return res;
}

template<RealView R>
real real::exp(const R &p) const {
    return gexp(*this, p);
}

template<RealView L>
template<RealView R>
real abs_rview<L>::exp(const R &p) const {
    return gexp(*this, p);
}

template<RealView L>
template<RealView R>
real neg_rview<L>::exp(const R &p) const {
    return gexp(*this, p);
}

template<RealView R>
real gpower(const R &r, int k) {
    if (!k)
        return 1;
    real res;
    res.p = r.get_n().power(std::abs(k));
    res.q = r.get_q().power(std::abs(k));
    if (k < 0) {
        whole aux = std::move(res.p);
        res.p = std::move(res.q);
        res.q = std::move(aux);
    }
    res.normalise();
    return res;
}

real real::power(int k) const {
    return gpower(*this, k);
}

template<RealView L>
real abs_rview<L>::power(int k) const {
    return gpower(*this, k);
}

template<RealView L>
real neg_rview<L>::power(int k) const {
    return gpower(*this, k);
}


template<RealView L, RealView R>
real gsqrt(const L &l, const R &r) {
    real res = l;
    while (mov_abs((res * res - l)) > r) {
        res = (res + l / res) / 2; 
    }
    res.normalise();
    return res;
}

template<RealView R>
real real::sqrt(const R &p) const {
    return gsqrt(*this, p);
}

template<RealView L>
template<RealView R>
real abs_rview<L>::sqrt(const R &p) const {
    return gsqrt(*this, p);
}

template<RealView L>
template<RealView R>
real neg_rview<L>::sqrt(const R &p) const {
    return gsqrt(*this, p);
}


template<RealView L, RealView R>
real glog1p(const L &l, const R &r) {
    real res = l; 
    real term = l;
    abs_rview<real> abs_term(&term);
    int n = 2;
    while (abs_term > r) {
        term *= -l * (n - 1) / n;
        res += term;
        n++;
    }
    res.normalise();
    return res;
}

template<RealView R>
real real::log1p(const R &p) const {
    return glog1p(*this, p);
}

template<RealView L>
template<RealView R>
real abs_rview<L>::log1p(const R &p) const {
    return glog1p(*this, p);
}

template<RealView L>
template<RealView R>
real neg_rview<L>::log1p(const R &p) const {
    return glog1p(*this, p);
}

template<RealView R>
void gprint(const R &r) {
    std::cout << r.is_neg() << "\n";
    r.get_n().printd();
    r.get_q().printd();
    std::cout << "--------------------" << std::endl;
}


void real::print() const {
    return gprint(*this);
}

template<RealView L>
void abs_rview<L>::print() const {
    std::cout << "abs view" << std::endl;
    return gprint(*this);
}

template<RealView L>
void neg_rview<L>::print() const {
    std::cout << "neg view" << std::endl;
    return gprint(*this);
}

auto real::abs() const {
    return abs_view(*this);
}

template <WholeView W>
abs_wview<W>::operator whole() const {
    whole res;
    res.p = get_n();
    res.neg = false;
    return res;
}

template <RealView R> 
abs_rview<R>::operator real() const {
    real res;
    res.p.p = this->get_p();
    res.p.neg = false;
    res.q = get_q();
    return res;
}

template <WholeView W>
neg_wview<W>::operator whole() const {
    whole res;
    res.p = get_n();
    res.neg = is_neg();
    return res;
}

template <RealView R> 
neg_rview<R>::operator real() const {
    real res;
    res.p.p = this->get_n();
    res.p.neg = this->is_neg();
    res.q = get_q();
    return res;
}

bool real::final = true;

int main() {
    real idk(123456789.0);
    idk.print();

    static_assert(WholeView<natural>, "balls");

    static_assert(WholeView<whole>, "balls");

    static_assert(RealView<real>, "balls");

    static_assert(WholeView<abs_wview<whole>>, "balls");
    static_assert(RealView<abs_rview<real>>, "balls");

    static_assert(WholeView<neg_wview<whole>>, "balls");
    static_assert(RealView<neg_rview<real>>, "balls");

    static_assert(!RealView<natural>, "balls");

    real two(2.0);
    real zero = 0;
    //std::cout << "zero" << std::endl;
    //zero.print();
    real one = 1;
    //std::cout << "one" << std::endl;
    //one.print();
    real ten = 10;
    //std::cout << "ten" << std::endl;
    //ten.print();
    real half = one / 2;
    //std::cout << "half" << std::endl;
    //half.print();

    real eps = ten.power( -3 );
    //std::cout << "eps" << std::endl;
    //eps.print();

    assert(ten > one);
    ten * -3;

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
    real aux = one.abs();
    aux = -one;

    assert( static_cast< real >( 1.0 ) == one );

    std::cout << "1 + -1" << std::endl;
    (one + -one).print();
    assert( one + -one == zero );

    std::cout << "1 * 10" << std::endl;
    (one * ten).print();
    assert( one * ten == ten );

    std::cout << "one exp" << std::endl;
    //(one.exp(eps) - e).abs().print();
    assert( ( one.exp( eps ) - e ).abs() < eps );

    std::cout << "log1p" << std::endl;
    zero.log1p(eps).abs().print();
    assert( zero.log1p( eps ).abs() < eps );

    std::cout << "h log1p" << std::endl;
    (half.log1p(eps) - l_half).abs().print();
    assert( ( half.log1p( eps ) - l_half ).abs() < eps );

    assert( ( ( one / two ).exp( eps ) - e.sqrt( eps ) ).abs() < 2 * eps );

    real three {3};
    assert(two > one);
    assert(two >= one);
    assert(-two < -one);

    for (int64_t i = 1; i < 10; ++i) {
        real a { real(13) / real(3) };
        real b { real(9) / real(4)};
        real iter{i};
        b = b * b + a + iter;
        a = a * a + b;
        assert(b < a);
        assert(b <= a);
        assert(a > b);
        assert(a >= b);
        assert(a != b);
        assert(b != a);
        assert(b == b);
        assert(b <= b);
        assert(b >= b);
        assert(!(a < b));
        assert(!(a <= b));
        assert(!(b > a));
        assert(!(b >= a));
        assert(!(a == b));
        assert(!(a != a));
        assert(!(a < a));
        assert(!(a > a));
        assert(a < a + iter);
        assert(a > a - iter);
        assert(-a < iter - a);
        assert(a - iter < a);
        assert(a + iter > a);
        assert(a + iter > a - iter);
        assert(a + a > a);
        assert(a - b > b);
        assert(b - a < b);
        assert(a * a == a * a + 0);
    }
    return 0;
}
