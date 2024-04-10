#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>


/* Vaším úkolem je tentokrát naprogramovat strukturu, která bude
 * reprezentovat libovolně velké přirozené číslo (včetně nuly). Tyto
 * hodnoty musí být možné:
 *
 *  • sčítat (operátorem ‹+›),
 *  • odečítat (‹x - y› je ovšem definováno pouze za předpokladu ‹x ≥ y›),
 *  • násobit (operátorem ‹*›),
 *  • libovolně srovnávat (operátory ‹==›, ‹!=›, ‹<›, atd.),
 *  • mocnit na kladný exponent typu ‹int› metodou ‹power›,
 *  • sestrojit z libovolné nezáporné hodnoty typu ‹int›.
 *
 * Implicitně sestrojená hodnota typu ‹natural› reprezentuje nulu.
 * Všechny operace krom násobení musí být nejvýše lineární vůči
 * «počtu dvojkových cifer» většího z reprezentovaných čísel.
 * Násobení může mít v nejhorším případě složitost přímo úměrnou
 * součinu ⟦m⋅n⟧ (kde ⟦m⟧ a ⟦n⟧ jsou počty cifer operandů). */
const uint64_t UP   = 0xffffffff00000000;
const uint64_t DOWN = 0x00000000ffffffff;

// stands for up_to_down
inline uint64_t utd(uint64_t x) {
    return (x & UP) >> 32;
}

struct carry {
    uint64_t val = 0;

    carry() = default;
    carry(uint64_t val) : val{val} {}

    void eval(uint64_t x, uint64_t &subres, std::vector<uint64_t> &t) {
        subres = x + val;
        t.push_back(subres & DOWN);
        val = utd(subres);
    }

    operator uint64_t() const {
        return val;
    }
};

struct natural {
    std::vector<uint64_t> n;

    natural(int x = 0) {
        assert(x >= 0);
        auto x64 = static_cast<uint64_t>(x);
        n = {x64 & DOWN};
        if (x64 > DOWN)
            n.push_back(utd(x64));
    }

    natural(uint64_t x) {
        n = {x & DOWN};
        if (x > DOWN)
            n.push_back((utd(x)));
    }

    void printn() const {
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

    void purgeZeroes() {
        while (!n.back() && len() > 1)
            pop();
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

    natural operator+(const natural &r) {
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

    friend natural dac_sum(const std::vector<natural> &subs, size_t i, size_t j) {
        if (i == j)
            return subs[i];
        size_t mid = (i + j) / 2;
        natural l = dac_sum(subs, i, mid);
        natural r = dac_sum(subs, mid + 1, j);
        return l + r;
    }

    friend natural operator*(const natural &l, const natural &r) {
        carry carry;
        uint64_t subres = 0;
        std::vector<natural> subreses (l.len());
        subreses[0].pop();
        for (size_t i = 1; i < l.len(); ++i)
            for (size_t j = 0; j < i - 1; ++j)
                subreses[i].push(0);
        for (size_t i = 0; i < l.len(); ++i) {
            carry = 0;
            for (size_t j = 0; j < r.len(); ++j) {
                carry.eval(l[i] * r[j], subres, subreses[i].n);
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
        natural res = this->power(x / 2);
        if (x % 2 == 0)
            return res * res;
        return (*this) * res * res;
    }

};

int main()
{
    natural tf(25);
    assert(natural(55).power(1) == natural(55));
    assert(tf.power(4) == natural(390625));
    natural zero;
    assert( zero + zero == zero );
    assert( zero * zero == zero );
    assert( zero - zero == zero );
    natural one( 1 );
    assert( one + zero == one );
    assert( one.power( 2 ) == one );
    natural test;
    for (int i = 0; i < 10; i++)
        test.push(0);
    test.push(1);
    assert(test * one == test);
    assert(test * zero == zero);
    assert(test + zero == test);
    test = 1;
    test.push(1);
    assert(test - natural(2) == natural(DOWN));

    return 0;
}
