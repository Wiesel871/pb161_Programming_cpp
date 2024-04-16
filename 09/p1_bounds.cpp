#include <cassert>
#include <map>
#include <cstdint>
#include <variant>

/* Implementujte třídu ‹bounds›, která si bude pro každý zadaný
 * celočíselný klíč pamatovat rozsah povolených hodnot. Přitom mez
 * typu ‹unbounded› bude znamenat, že v daném směru není hodnota
 * příslušná danému klíči nijak omezena. */

struct unbounded {};
struct undefined {};

/* Samotná třída bounds bude mít metody:
 *
 *  • ‹set_lower( k, b )› nastaví spodní mez ‹b› pro klíč ‹k› (‹b›
 *    je buď 64b celé číslo, nebo hodnota ‹unbounded{}›),
 *  • ‹set_upper( k, b )› obdobně, ale horní mez,
 *  • ‹set_default_lower( b )› nastaví implicitní dolní mez (platí
 *    pro klíče, kterým nebyla nastavena žádná jiná),
 *  • ‹set_default_upper( b )› obdobně, ale horní mez,
 *  • ‹valid( k, i )› vrátí ‹true› právě když hodnota ‹i› spadá do
 *    mezí platných pro klíč ‹k›.
 *
 * Všechny takto zadané intervaly jsou oboustranně otevřené. */

struct bound {
    std::variant<int64_t, unbounded, undefined> var;

    bound() {
        var = undefined{};
    }

    bound &operator=(int64_t val) {
        var = val;
        return *this;
    } 

    bound &operator=(unbounded) {
        var = unbounded{};
        return *this;
    }

    std::variant<int64_t, unbounded, undefined> *addr() {
        return &var;
    }
};

struct bounds {

    struct inner {
        bound lower, upper;
    };

    bound d_lower, d_upper;

    std::map<int, inner> keys; 

    bounds() = default;

    void set_lower(int k, auto val) {
        keys[k].lower = val;
    }

    void set_upper(int k, auto val) {
        keys[k].upper = val;
    }

    void set_default_lower(auto val) {
        d_lower = val;
    }

    void set_default_upper(auto val) {
        d_upper = val;
    }

    bool valid(int k, int64_t val) {
        return below(k, val) && above(k, val);
    }

    private:

    bool above(int k, int64_t val) {
        if (int64_t *p = std::get_if<int64_t>(keys[k].lower.addr()); p) {
            return val > *p;
        }
        if (undefined *p = std::get_if<undefined>(keys[k].lower.addr()); p) {
            if (int64_t *d = std::get_if<int64_t>(d_lower.addr()); d) {
                return val > *d;
            }
        }
        return true;
    }

    bool below(int k, int64_t val) {
        if (int64_t *p = std::get_if<int64_t>(keys[k].upper.addr()); p) {
            return val < *p;
        }
        if (undefined *p = std::get_if<undefined>(keys[k].upper.addr()); p) {
            if (int64_t *d = std::get_if<int64_t>(d_upper.addr()); d) {
                return val < *d;
            }
        }
        return true;
    }
};

int main()
{
    bounds a;
    a.set_lower( 0, -10 );
    a.set_upper( 0, unbounded{} );
    assert( a.valid( 0, 0 ) );
    assert( a.valid( 0, -9 ) );
    assert( a.valid( 0, 21351 ) );
    assert( !a.valid( 0, -21351 ) );

    a.set_upper( 1, 100 );
    assert( a.valid( 1, -13213 ) );

    assert( a.valid( 2, ( 1ll << 62 ) + 1 ) );
    assert( a.valid( 2, ( 1ll << 62 ) * -1 ) );

    bounds b;
    b.set_default_lower( -5 );
    b.set_default_upper( 15 );
    assert( !b.valid( 0, 65 ) );
    assert( b.valid( 0, -2 ) );

    return 0;
}
