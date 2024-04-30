#include <cassert>
#include <cmath>
#include <sstream>
#include <string_view>
#include <utility>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <charconv>

/* Ve cvičení ‹07/p6_linear› jsme napsali jednoduchý program, který
 * řeší systémy lineárních rovnic o dvou neznámých. Tento program
 * nyní rozšíříme o načítání vstupu z řetězce. */

/* Naprogramujte čistou funkci ‹solve›, která má jediný parametr
 * (řetězec předaný jako hodnota typu ‹std::string_view›). Vstup
 * obsahuje právě dvě rovnice, na každém řádku jedna, se dvěma
 * jednopísmennými proměnnými a celočíselnými koeficienty. Každý
 * člen je oddělen od operátorů (‹+› a ‹-›) a znaku rovnosti
 * mezerami, jednotlivý člen (koeficient včetně znaménka a případná
 * proměnná) naopak mezery neobsahuje. Není-li vstup v očekávaném
 * formátu, situaci řešte jak uznáte za vhodné (můžete např. ukončit
 * funkci výjimkou ‹no_parse›).
 *
 * Výsledkem bude dvojice čísel typu ‹double›. Pořadí výsledku nechť
 * je abecední (např. dvojice ‹x›, ‹y›). Jinak se funkce ‹solve›
 * chová stejně, jak je popsáno v zmiňovaném příkladu
 * ‹07/p6_linear›. */
struct no_solution : std::exception {};

bool eq(double a, double b) {
    return std::fabs(a - b) <= ( (std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * 1.0e-5f);
}

struct row {
    double x, y, d;

    row() : x{0}, y{0}, d{0} {}

    row(double d) : d{d} {}

    row(double x, double y, double d) : x{x}, y{y}, d{d} {}

    row(std::string_view word, char one, char two) {
        double *target = nullptr;
        std::cout << "input: " << word << " " << one << " " << two << std::endl;
        if (*(word.end() - 1) == one) {
            word.remove_suffix(1);
            target = &x;
            y = 0;
            d = 0;
            std::cout << "x: ";
        } else if (*(word.end() - 1) == two) {
            word.remove_suffix(1);
            target = &y;
            x = 0;
            d = 0;
            std::cout << "y: ";
        } else {
            target = &d;
            x = 0;
            y = 0;
            std::cout << "d: ";
        }
        if (word.empty()) {
            *target = 1;
        } else {
            std::string aux(word);
            std::istringstream i(aux);
            i >> (*target);
        }
        std::cout << (*target) << std::endl;
    }

    row operator+(const row &r) const {
        return {x + r.x, y + r.y, d + r.d};
    }

    void print() const {
        std::cout << x << " " << y << " " << d << std::endl;
    }

    row operator+(int r) const {
        return {x, y, d + r};
    }

    row operator+(double r) const {
        return {x, y, d + r};
    }

    row operator==(const row &r) const {
        return {x - r.x, y - r.y, d - r.d};    
    }

    row operator==(int r) const {
        return {x, y, d - r};
    }

    row operator==(double r) const {
        return {x, y, d - r};
    }

    row operator-(const row &r) const {
        std::cout << "-" << std::endl;
        print();
        r.print();
        return {x - r.x, y - r.y, d - r.d};    
    }

    row operator-(int r) const {
        return {x, y, d - r};
    }

    row operator-(double r) const {
        return {x, y, d - r};
    }

    row operator*(double r) const {
        return {r * x, r * y, r * d};
    }

    row operator*(int r) const {
        return {r * x, r * y, r * d};
    }

    friend row operator*(double l, const row &r) {
        return {l * r.x, l * r.y, l * r.d};
    }

    friend row operator*(int l, const row &r) {
        return {l * r.x, l * r.y, l * r.d};
    }

    row operator/(double r) const {
        return {x / r, y / r, d / r};
    }

    row operator/(int r) const {
        return {x / r, y / r, d / r};
    }
};

const row x {1, 0, 0};
const row y {0, 1, 0};

inline void check(const row &r) {
    if (eq(r.x, 0) && eq(r.y, 0) && !eq(r.d, 0))
        throw no_solution{};
}

std::pair<double, double> solve(const row &r1, const row &r2) {
    check(r1);
    check(r2);
    std::cout << "solve:" << std::endl;
    r1.print();
    r2.print();
    std::pair<double, double> res;
    double det_a = (r1.x * r2.y - r1.y * r2.x);
    if (eq(det_a, 0))
        throw no_solution{};
    double det_a1 = (r1.d * r2.y - r1.y * r2.d);
    double det_a2 = (r1.x * r2.d - r1.d * r2.x);
    res.first = det_a1 / det_a;
    res.second = det_a2 / det_a;
    std::cout << "pre: " << res.first << " " << res.second << std::endl;
    if (res.first < 0 && res.second > 0) {
        res.first = -res.first;
        res.second = -res.second;
    }
    if (eq(res.first, 0))
        res.first = std::abs(res.first);
    if (eq(res.second, 0))
        res.second = std::abs(res.second);
    std::cout << "res: " << res.first << " " << res.second << std::endl;
    return res;
}

struct no_parse    : std::exception {};


std::vector< std::string_view > words( std::string_view strv) {
    std::vector<std::string_view> res = {};
    auto beg = strv.begin();
    auto end = beg;
    while (end != strv.end()) {
        if (isspace(*end)) {
            if (beg != end) {
                res.emplace_back(beg, end);
                beg = end;
            } else {
                ++beg;
                ++end;
            }
        } else {
            ++end;
        }
    }
    if (beg != end)
        res.emplace_back(beg, end);
    return res;
}

row parse_wrs(std::vector<std::string_view> wrs, char one, char two) {
    enum class sign {
        p,
        m,
    } s = sign::p;
    row res;
    for (const auto &w: wrs) {
        if (w[0] == '+') {
            s = sign::p;
        } else if (w[0] == '-') {
            if (w.size() > 1) {
                res = res - row(w, one, two);
            } else {
                s = sign::m;
            }
        } else {
            if (s == sign::m) {
                res = res - row(w, one, two);
            } else {
                res = res + row(w, one, two);
            }
        }
    }
    return res;
}

row parse_line(std::string_view str) {
    auto wrs = words(str);
    enum class last {
        var,
        op,
    };
    std::cout << "line: " << str << std::endl;
    last ls = last::op;
    char one = '\0', two = '\0';
    bool after_eq = false;
    std::vector<std::string_view> l = {}, r = {};
    for (const auto &w: wrs) {
        std::cout << "word: " << w << std::endl;
        if (ls == last::op) {
            ls = last::var;
            auto var = std::find_if(w.begin(), w.end(), [](int c) {return isalpha(c);});
            if (var == w.end())
                // ja vim, lepsi ma realne nenapadlo
                goto push;
            if (var != w.end() - 1)
                throw no_parse();
            if (one && two && *var != one && *var != two)
                throw no_parse();
            if (one && *var != one)
                two = *var;
            if (!one)
                one = *var;
        } else {
            ls = last::op;
            if (w.size() != 1)
                throw no_parse();
            if (w[0] == '=') {
                if (after_eq)
                    throw no_parse();
                after_eq = true;
                continue;
            }
            if (w[0] != '-' && w[0] != '+')
                throw no_parse();
        }
push:
        if (after_eq)
            r.push_back(w);
        else
            l.push_back(w);
    }
    if (one > two)
        std::swap(one, two);
    assert(!l.empty());
    assert(!r.empty());
    row lr = parse_wrs(l, one, two);
    row rr = parse_wrs(r, one, two);
    return lr == rr;
}

std::pair< double, double > solve( std::string_view eq ) {
    if (std::count(eq.begin(), eq.end(), '\n') != 2)
        throw no_parse();
    std::size_t line_sep = eq.find_first_of('\n');
    auto l = eq.substr(0, line_sep);
    auto r = eq.substr(line_sep + 1, eq.size());
    row lr = parse_line(l);
    row rr = parse_line(r);
    return solve(lr, rr);
}

int main()
{
    std::pair a( 1.0, 0.0 ), b( -1.0, 0.0 ), c( .6, -.2 );

    auto check = []( auto s, auto val )
    {
        auto [ u, v ] = solve( s );
        auto [ x, y ] = val;
        return std::abs( u - x ) < 1e-6 &&
               std::abs( v - y ) < 1e-6;
    };

    assert( check( " x + y     =  1\n x -  y     =  1\n", a ) );
    assert( check( " x + y     = -1\n x -  y     = -1\n", b ) );
    assert( check( " x + y     =  x\n x +  y     =  1\n", a ) );
    assert( check( "2x + y     =  1\n x - 2y     =  1\n", c ) );
    assert( check( " x + y - 1 =  0\n x -  y - 1 =  0\n", a ) );
    assert( check( "10x + 10y  = 10\n x -  y     =  1\n", a ) );

    try
    {
        solve( "x + y = 1\nx + y = 2\n" );
        assert( false );
    }
    catch ( const no_solution & ) {}

    return 0;
}
