#include <cmath>
#include <vector>
#include <tuple>
#include <cassert>

/* † Navrhněte typ, který se bude navenek chovat jako sekvenční
 * kontejner dvojic (‹std::tuple›) čísel, ale vnitřně bude data
 * uchovávat ve dvojici kontejnerů (‹std::vector›) celých čísel.
 * Požadované metody:
 *
 *  • ‹begin›, ‹end› a odpovídající zjednodušený iterátor s:
 *    ◦ operátory ‹==› a ‹!=›,
 *    ◦ prefixovým operátorem ‹++›,
 *    ◦ operátorem dereference (unární ‹*›),
 *    kde výsledek operátoru ‹*› musí být použitelný jako dvojice
 *    čísel, včetně ‹std::get› a přiřazení do jednotlivých složek.
 *  • ‹size›,
 *  • ‹push_back›,
 *  • ‹emplace_back› (se dvěma parametry typu ‹int›, nebo
 *    žádným parametrem),
 *  • ‹left› a ‹right› vrátí konstantní referenci na příslušný
 *    vnitřní kontejner (konstantní proto, aby nebylo jejich
 *    použitím možné porušit potřebné invarianty).
 *
 * Stačí, aby iterace fungovala pro nekonstantní hodnoty typu
 * ‹components› (naprogramovat konstantní i nekonstantní iteraci bez
 * duplikace kódu neumíme).
 *
 * Nápověda: zvažte, jak využít ‹std::tuple› s vhodnými parametry.
 * K vyřešení příkladu stačí už probrané konstrukce. */

struct components {
    std::vector<int> l = {};
    std::vector<int> r = {};

    struct iterator {
        std::size_t i = 0;
        std::vector<int> *l;
        std::vector<int> *r;

        iterator &operator++() {
            ++i;
            return *this;
        }

        std::tuple<int &, int &> operator*() {
            return {(*l)[i], (*r)[i]};
        }

        bool operator!=(const iterator &ri) const {
            return i != ri.i || l != ri.l || r != ri.r;
        }

        bool operator==(const iterator &ri) const {
            return i == ri.i || l == ri.l || r == ri.r;
        }
    };

    friend iterator;

    iterator begin() {
        return {0, &l, &r};
    }

    iterator end() {
        return {size(), &l, &r};
    }

    std::size_t size() const {
        return l.size();
    }

    const std::vector<int> &left() const {
        return l;
    }

    const std::vector<int> &right() const {
        return r;
    }

    void push_back(const std::tuple<int, int> &t) {
        l.push_back(std::get<0>(t));
        r.push_back(std::get<1>(t));
    }

    void emplace_back(int li, int ri) {
        l.push_back(li);
        r.push_back(ri);
    }
};

int main()
{
    components x;
    const components &x_const = x;
    const auto &l = x_const.left();
    const auto &r = x_const.right();

    x.emplace_back( 1, 3 );
    assert(( l == std::vector{ 1 } ));
    assert(( r == std::vector{ 3 } ));
    assert(( *x.begin() == std::tuple{ 1, 3 } ));

    assert( x_const.size() == 1 );
    x.push_back( { 3, 3 } );
    unsigned i = 0;

    for ( auto [ a, b ] : x )
    {
        assert( a == 1 || a == 3 );
        assert( b == 3 );
        ++ i;
    }

    assert( x.size() == i );
    std::get< 0 >( *x.begin() ) = 3;
    assert(( l == std::vector{ 3, 3 } ));

    return 0;
}
