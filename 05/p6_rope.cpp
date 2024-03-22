#include <cassert>
#include <cstddef>
#include <cstdio>
#include <vector>
#include <stack>

/* Lano je datová struktura, která reprezentuje sekvenci,
 * implementovaná jako binární strom, který má v listech klasická
 * pole a ve vnitřních uzlech udržuje celočíselné váhy. Sdílení
 * podstromů je dovolené a očekávané.
 *
 * Váhou uzlu se myslí celková délka sekvence reprezentovaná jeho
 * levým podstromem. Díky tomu lze lana spojovat a indexovat v čase
 * lineárním k «hloubce» stromu.¹
 *
 * Naprogramujte:
 *
 *  • konstruktor, který vytvoří jednouzlové lano z vektoru,
 *  • konstruktor, který spojí dvě stávající lana,
 *  • metodu ‹get( i )›, která získá ‹i›-tý prvek,
 *  • a ‹set( i, value )›, která ‹i›-tý prvek nastaví na ‹value›.
 *
 * Pro účely tohoto příkladu není potřeba implementovat žádnou formu
 * vyvažování.
 *
 * ¹ Spojení dvou lan lze za cenu dodatečné informace v uzlech, nebo
 *   pomalejší indexace, provést i v konstantním čase. */

enum Tag {
    Leaf = 'l',
    Concat = 'c',
};

/*
void print_vec(const std::vector<int> &v) {
    for (int x:v) {
        printf("%d ", x);
    }
    printf("\n");
}

int vec_sum(const std::vector<int> &v) {
    int res = 0;
    for (int x: v) {
        res += x;
    }
    return res;
}
*/

struct rope {
    Tag t;
    rope *l = nullptr;
    rope *r = nullptr;
    std::vector<int> rep = {};
    std::size_t len_l;
    std::size_t len_f;
    
    rope(std::vector<int> rep) : t{Leaf}, rep{rep} {
        len_f = len_l = rep.size();
    }

    rope(rope &l, rope &r) : t{Concat}, l{&l}, r{&r} {
        len_l = l.len_f;
        len_f = len_l + r.len_f;
    }
    
    char get(std::size_t i) const {
        std::stack<const rope *> st = {};
        st.push(this);
        while (!st.empty() && st.top()->t != Leaf) {
            const rope *cur = st.top();
            st.pop();
            if (i < cur->len_l) {
                st.push(cur->l);
            } else {
                i -= cur->len_l;
                st.push(cur->r);
            }
        }
        return st.top()->rep[i];
    }

    void set(std::size_t i, int c) {
        std::stack<rope *> st = {};
        st.push(this);
        while (!st.empty() && st.top()->t != Leaf) {
            rope *cur = st.top();
            st.pop();
            if (i < cur->len_l) {
                st.push(cur->l);
            } else {
                i -= cur->len_l;
                st.push(cur->r);
            }
        }
        st.top()->rep[i] = c;
    }

    /*
    void print(std::size_t i = 1) const {
        switch (t) {
            case Leaf:
                print_vec(rep);
                break;
            case Concat:
                l->print(i + 1);
                for (std::size_t j = 0; j < i; ++j)
                    printf("-");
                printf("\n");
                r->print(i + 1);
        }
    }

    int sum() const {
        switch (t) {
            case Leaf:
                return vec_sum(rep);
            case Concat:
                return l->sum() + r->sum();
        }
        assert(false);
    }
    */
};


int main()
{
    rope h( { 104, 101, 108, 108, 111 } ),
         w( { 119, 111, 114, 108, 100 } );

    assert( h.get( 0 ) == 104 );
    assert( h.get( 4 ) == 111 );
    assert( w.get( 0 ) == 119 );
    assert( w.get( 4 ) == 100 );

    rope hw( h, w );
    assert( hw.get( 0 ) == 104 );
    assert( hw.get( 4 ) == 111 );
    assert( hw.get( 5 ) == 119 );
    assert( hw.get( 9 ) == 100 );

    rope hww( hw, w );
    assert( hww.get(  0 ) == 104 );
    assert( hww.get(  4 ) == 111 );
    assert( hww.get(  5 ) == 119 );
    assert( hww.get(  9 ) == 100 );
    assert( hww.get( 10 ) == 119 );
    assert( hww.get( 14 ) == 100 );

    const rope hhw( h, hw );
    assert( hhw.get( 0 ) == 104 );
    assert( hhw.get( 4 ) == 111 );
    assert( hhw.get( 5 ) == 104 );
    assert( hhw.get( 9 ) == 111 );
    assert( hhw.get( 10 ) == 119 );
    assert( hhw.get( 14 ) == 100 );

    h.set( 0, 120 );
    assert( h.get( 0 ) == 120 );
    assert( h.get( 4 ) == 111 );
    assert( w.get( 0 ) == 119 );
    assert( w.get( 4 ) == 100 );

    /* subtrees are shared */
    assert( hhw.get( 0 ) == 'x' );

    /*
    rope *p = new rope({-1, 33}), *q = new rope({-3, -1, 100});
    p = new rope(*p, *q);
    p->print();
    int sub = p->sum();
    printf("%d\n", sub);
    assert(sub == 64);
    delete p->l;
    delete p->r;
    delete p;
    */
    return 0;
}
