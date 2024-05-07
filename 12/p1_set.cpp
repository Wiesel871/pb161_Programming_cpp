#include <cassert>
#include <cstdlib>
#include <istream>
#include <sstream>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

/* Implementujte typ ‹set›, který reprezentuje množinu libovolných
 * celých čísel, s operacemi:
 *
 *  • ‹add› – přidá prvek,
 *  • ‹has› – ověří přítomnost prvku,
 *  • ‹size› – vrátí počet prvků.
 *
 * Dále nechť je hodnoty typu ‹set› možné číst z a zapisovat do
 * vstupně-výstupních proudů. Na výstupu budou mít množiny tuto
 * formu:
 *
 *     {}
 *     { 1 }
 *     { 1, 2 }
 *
 * Na vstupu akceptujte i varianty s jiným počtem bílých znaků
 * (včetně žádných). */

struct set {
    std::set<int> in;

    void add(int x) {
        in.insert(x);
    }

    bool has(int x) const {
        return in.contains(x);
    }

    std::size_t size() const {
        return in.size();
    }

    friend std::istream &operator>>(std::istream &is, set &s) {
        std::vector<std::string> set_str = {""};
        s.in = {};
        char c = 0;
        while (c != '{')
            is >> c;
        is >> c;
        while (c != '}') {
            if (c == ',')
                set_str.emplace_back();
            else
                set_str.back().push_back(c);
            is >> c;
        }
        if (std::find_if(set_str.back().begin(), set_str.back().end(), [](char c){ return std::isdigit(c); }) == set_str.back().end())
            set_str.pop_back();
        for (const auto &str: set_str) {
            s.add(std::atoi(str.data()));
        }
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const set &s) {
        os << "{";
        for (auto it = s.in.begin(); it != s.in.end(); ++it) {
            os << " " << *it << ((++it == s.in.end()) ? " " : ",");
            --it;
        }
        return os << "}";
    }
};

int main()
{
    // clang-tidy: -readability-container-size-empty

    std::stringstream s( "{3} {} {1} {1 } {1 ,2}" );
    set m;
    const auto &const_m = m;

    s >> m;
    assert( s && m.size() == 1 && m.has( 3 ) );
    s >> m;
    assert( s && const_m.size() == 0 && !m.has( 3 ) );
    s >> m;
    assert( s && m.size() == 1 && m.has( 1 ) );
    s >> m;
    assert( s && m.size() == 1 && const_m.has( 1 ) );
    s >> m;
    assert( s && m.size() == 2 && m.has( 1 ) && m.has( 2 ) );

    set n, o;
    n.add( 1 );

    s.str( "" );
    s.clear();

    s << const_m << " " << n << " " << o;
    assert( s.str() == "{ 1, 2 } { 1 } {}" );

    return 0;
}
