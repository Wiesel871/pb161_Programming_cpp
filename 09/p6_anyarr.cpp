#include <any>
#include <cassert>
#include <vector>

/* Naprogramujte typ ‹any_array›, který bude reprezentovat dynamické
 * pole libovolných hodnot, a bude mít tyto metody:
 *
 *  • ‹size› – vrátí počet uložených hodnot,
 *  • ‹append› – přijme hodnotu libovolného typu a vloží ji
 *    na konec pole,
 *  • ‹transform_int› – přijme libovolnou unární funkci
 *    ‹int f( int )›, a každou uloženou hodnotu ‹x› typu ‹int›
 *    upraví na ‹f( x )› (přitom ostatní hodnoty nezmění),
 *  • ‹remove_integers› – odstraní hodnoty typu ‹int›,
 *  • ‹remove_floats› – odstraní hodnoty typu ‹float› a ‹double›,
 *  • ‹equals› – přijme index ‹i› a hodnotu libovolného typu ‹v›
 *    a vrátí ‹true› je-li na indexu ‹i› uložena hodnota stejného
 *    typu jako ‹v› a tyto hodnoty se rovnají.
 *
 * Metody ‹remove_integers› a ‹remove_floats› musí mít nejvýše
 * lineární časovou složitost, zatímco metoda ‹equals› konstantní. */

using aar = std::vector<std::any>;

struct any_array {
    aar ar = {};

    std::size_t size() const {
        return ar.size();
    }

    void append(auto x) {
        ar.emplace_back(x);
    }

    void transform_int(auto f) {
        for (auto &x: ar) {
            if (x.type() == typeid(int)) {
                x = f(std::any_cast<int>(x));
            }
        }
    }

    bool equals(int i, const auto &v) const {
        return ar[i].type() == typeid(decltype(v)) && std::any_cast<decltype(v)>(ar[i]) == v;
    }

    void remove_integers() {
        aar aux = {};
        for (const auto &x: ar)
            if (x.type() != typeid(int))
                aux.emplace_back(x);
        ar = std::move(aux);
    }

    void remove_floats() {
        aar aux = {};
        for (const auto &x: ar)
            if (x.type() != typeid(double))
                aux.emplace_back(x);
        ar = std::move(aux);
    }
};

int main()
{
    any_array arr;
    const any_array &c_arr = arr;

    arr.append( 3 );
    assert( c_arr.equals( 0, 3 ) );
    assert( !arr.equals( 0, 3.0 ) );

    arr.transform_int( []( int x ) { return x - 2; } );

    arr.remove_floats();
    assert( arr.equals( 0, 1 ) );

    arr.append( 3.3 );
    assert( arr.equals( 1, 3.3 ) );
    arr.remove_integers();
    assert( arr.equals( 0, 3.3 ) );
    arr.remove_floats();
    assert( c_arr.size() == 0 );

    return 0;
}
