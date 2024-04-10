#include <cassert>
#include <utility>
#include <map>
#include <cstdint>

/* Implementujte třídu ‹bounds›, která si bude pro každý zadaný
 * celočíselný klíč pamatovat rozsah povolených hodnot. Přitom mez
 * typu ‹unbounded› bude znamenat, že v daném směru není hodnota
 * příslušná danému klíči nijak omezena. */

struct unbounded {};

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

struct bounds {
    enum type {
        u, b, ub, bu
    };

    type combine(const type &l, const type &r) {
        switch (l) {
            case bu: 
                switch (r) {
                        case u: return u;
                        case b: return b;
                    } assert(false);
            case ub:
            default: assert(false);
        }
    }

    struct inner {
        type t;
        int64_t lo;
        int64_t up;
    };

    type def = u;
    int64_t dl = 0, du = 0;

    std::map<int, inner> keys; 

    bounds() = default;

    void swap_up(int k, type &t, int64_t &nb) {
        t = keys[k].t;
        nb = keys[k].up;
    }

    void swap_lo(int k, type &t, int64_t &nb) {
        t = keys[k].t;
        nb = keys[k].lo;
    }

    void set_lower(int k, int64_t nb) {
        type t = def;
        int64_t up = du;
        if (keys.contains(k))
            swap_up(k, t, up);
        switch (t) {
            case u: case bu: keys[k] = {bu, nb, 0}; break;
            case b: case ub: keys[k] = {b, nb, up};  
        }
    }

    void set_lower(int k, unbounded) {
        type t = def;
        int64_t up = du;
        if (keys.contains(k)) {
            swap_up(k, t, up);
        }
        switch (t) {
            case u: case bu: keys[k] = {u, 0, 0}; break;
            case b: case ub: keys[k] = {ub, 0, up};
        }
    }

    void set_upper(int k, int64_t nb) {
        type t = def;
        int64_t lo = dl;
        if (keys.contains(k))
            swap_lo(k, t, lo);
        switch (t) {
            case u: case ub: keys[k] = {ub, 0, nb}; break;
            case b: case bu: keys[k] = {b, lo, nb};
        }
    }

    void set_upper(int k, unbounded) {
        type t = def;
        int64_t lo = dl;
        if (keys.contains(k))
            swap_lo(k, t, lo);
        switch (t) {
            case u: case ub: keys[k] = {u, 0, 0}; break;
            case b: case bu: keys[k] = {bu, lo, 0};
        }
    }

    void set_default_lower(int64_t nb) {
        switch (def) {
            case u: case bu: def = bu; dl = nb; break;
            case b: case ub: def = b; dl = nb;
        }
    }

    void set_default_lower(unbounded) {
        switch (def) {
            case u: case bu: def = u; break;
            case b: case ub: def = ub;
        }
    }

    void set_default_upper(int64_t nb) {
        switch (def) {
            case u: case ub: def = ub; du = nb; break;
            case b: case bu: def = b; du = nb;
        }
    }

    void set_default_upper(unbounded) {
        switch (def) {
            case u: case bu: def = u; break;
            case b: case ub: def = ub; 
        }
    }

    bool valid(int k, int64_t val) {
        type t = def;
        int64_t lo = dl;
        int64_t up = du;
        if (keys.contains(k)) {
            swap_lo(k, t, lo);
            swap_up(k, t, up);
        }

        switch (t) {
            case u: return true;
            case bu: return val > lo;
            case ub: return val < up;
            case b: return val > lo && up;
        }
        assert(false);
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
