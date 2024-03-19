#include <cassert>
#include <cstddef>
#include <cstdio>
#include <vector>
#include <list>
#include <memory>

/* V tomto cvičení rozšíříme binární trie z ‹p2› – místo
 * posloupnosti bitů budeme za klíče brát posloupnosti celých čísel
 * typu ‹int›. Vylepšíme také rozhraní – místo ruční správy uzlů
 * poskytneme přímo operaci vložení zadaného klíče.
 *
 * Množiny budeme nadále kódovat do binárního stromu:
 *
 *  • levý potomek uzlu rozšiřuje reprezentovaný klíč o jedno celé
 *    číslo (podobně jako tomu bylo u binární trie) – toto číslo je
 *    tedy součástí levé hrany,
 *  • pravý „potomek“ uzlu je ve skutečnosti jeho sourozenec, a
 *    hrana není nijak označená (přechodem doprava se klíč nemění),
 *  • řetěz pravých potomků tvoří de-facto zřetězený seznam, který
 *    budeme udržovat seřazený podle hodnot na odpovídajících
 *    «levých» hranách.
 *
 *
 * Příklad: na obrázku je znázorněná trie s klíči [3, 1], [3, 13,
 * 7], [3, 15], [5, 2], [5, 5], [37]. Levý potomek je pod svým
 * rodičem, pravý je od něj napravo.
 *
 *   ●────────────────▶●─────────────▶●
 *   │                 │              │
 *   │ 3               │ 5            │ 37
 *   ▼                 ▼              ▼
 *   ●────▶●────▶●     ●─────▶●       ●
 *   │     │     │     │      │
 *   │ 1   │ 13  │ 15  │ 2    │ 5
 *   ▼     ▼     ▼     ▼      ▼
 *   ●     ●     ●     ●      ●
 *         │
 *         │ 7
 *         ▼
 *         ●
 *
 * Můžete si představit takto reprezentovanou trie jako ⟦2³²⟧-ární,
 * které by bylo zcela jistě nepraktické přímo implementovat. Proto
 * reprezentujeme virtuální uzly pomyslného ⟦2³²⟧-árního stromu jako
 * zřetězené seznamy pravých potomků ve fyzicky binárním stromě. */

using key = std::vector< int >;

struct sub_node;

struct trie_node {
    std::list<sub_node> rs = {};
};

struct sub_node {
    trie_node *l;
    int k;

    sub_node(int k) : k{k} {
        l = new trie_node;
    }

    void clear() {
        for (auto &x: l->rs) {
            x.clear();
        }
        delete l;
    }
};

/* Rozhraní typu ‹trie› je velmi jednoduché: má metodu ‹add›, která
 * přidá klíč a metodu ‹has›, která rozhodne, je-li daný klíč
 * přítomen. Obě jako parametr přijmou hodnotu typu ‹key›. Prefixy
 * vložených klíčů nepovažujeme za přítomné. */

struct trie {
    std::unique_ptr<trie_node> root = std::make_unique<trie_node>();

    void add(key k) {
        std::size_t i = 0;
        auto node = &root->rs;
        auto p = node->begin();
        while (i < k.size()) {
            if (node->empty() || p == node->end() || p->k > k[i]) {
                node = &(node->insert(p, {k[i]}))->l->rs;
                p = node->begin();
                ++i;
            } else if (p->k == k[i]) {
                node = &p->l->rs;
                p = node->begin();
                ++i;
            } else {
                ++p;
            }
        }
    }

    bool has(key k) const {
        std::size_t i = 0;
        auto node = &root->rs;
        auto p = node->begin();
        while (i < k.size()) {
            if (p == node->end())
                return false;
            if (k[i] == p->k) {
                ++i;
                node = &p->l->rs;
                p = node->begin();
            } else {
                ++p;
            }
        }
        return node->empty();
    }

    ~trie() {
        for (auto &x : root->rs) {
            x.clear();
        }
    }
};

int main()
{
    trie t;
    const trie &ct = t;
    assert( !ct.has( { 1 } ) );

    t.add( { 1 } );
    assert(  t.has( { 1 } ) );
    assert( !t.has( { 1, 1 } ) );

    /* { 1 } disappears from the trie, since it is a prefix of { 1, 1 } */

    t.add( { 1, 1 } );
    assert(  t.has( { 1, 1 } ) );
    assert( !t.has( { 1 } ) );

    /* add a right sibling to a non-root node */

    t.add( { 1, 2 } );
    assert(  t.has( { 1, 1 } ) );
    assert(  t.has( { 1, 2 } ) );
    assert( !t.has( { 2, 2 } ) );
    assert( !t.has( { 2, 1 } ) );

    /* add a right sibling to the root node */

    t.add( { 2, 1 } );
    assert(  t.has( { 1, 1 } ) );
    assert(  t.has( { 2, 1 } ) );
    assert(  t.has( { 1, 2 } ) );
    assert( !t.has( { 2, 2 } ) );

    /* add a left sibling to a non-root node */

    t.add( { 2, 0, 1 } );
    assert(  t.has( { 1, 1 } ) );
    assert(  t.has( { 2, 0, 1 } ) );
    assert(  t.has( { 2, 1 } ) );
    assert(  t.has( { 1, 2 } ) );

    /* add a left sibling to the root node */

    t.add( { 0, 1 } );
    assert(  t.has( { 1, 1 } ) );
    assert(  t.has( { 2, 1 } ) );
    assert(  t.has( { 1, 2 } ) );
    assert(  t.has( { 0, 1 } ) );
    assert(  t.has( { 2, 0, 1 } ) );

    return 0;
}
