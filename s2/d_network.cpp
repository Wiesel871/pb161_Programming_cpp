#include <cassert>
#include <cstddef>
#include <stack>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

/* Vaším úkolem bude tentokrát naprogramovat simulátor počítačové
 * sítě, s těmito třídami, které reprezentují propojitelné síťové
 * uzly:
 *
 *  • ‹endpoint› – koncový uzel, má jedno připojení k libovolnému
 *    jinému uzlu,
 *  • ‹bridge› – propojuje 2 nebo více dalších uzlů,
 *  • ‹router› – podobně jako bridge, ale každé připojení musí být
 *    v jiném segmentu.
 *
 * Dále bude existovat třída ‹network›, která reprezentuje síťový
 * segment jako celek. Každý uzel patří právě jednomu segmentu.
 * Je-li segment zničen, musí být zničeny (a odpojeny) i všechny
 * jeho uzly.
 *
 * Třída ‹network› bude mít tyto metody pro vytváření uzlů:
 *
 *  • ‹add_endpoint()› – vytvoří nový (zatím nepřipojený) koncový
 *    uzel, převezme jeho vlastnictví a vrátí vhodný ukazatel na
 *    něj,
 *  • ‹add_bridge( p )› – podobně pro ‹p›-portový bridge,
 *  • ‹add_router( i )› – podobně pro směrovač s ‹i› rozhraními.
 *
 * Jsou-li ‹m› a ‹n› libovolné typy uzlů, musí existovat vhodné
 * metody:
 *
 *  • ‹m->connect( n )› – propojí 2 uzly. Metoda je symetrická v tom
 *    smyslu, že ‹m->connect( n )› a ‹n->connect( m )› mají tentýž
 *    efekt. Metoda vrátí ‹true› v případě, že se propojení podařilo
 *    (zejména musí mít oba uzly volný port).
 *  • ‹m->disconnect( n )› – podobně, ale uzly rozpojí (vrací ‹true›
 *    v případě, že uzly byly skutečně propojené).
 *  • ‹m->reachable( n )› – ověří, zda může uzel ‹m› komunikovat
 *    s uzlem ‹n› (na libovolné vrstvě, tzn. včetně komunikace skrz
 *    routery; jedná se opět o symetrickou vlastnost; vrací hodnotu
 *    typu ‹bool›).
 *
 * Konečně třída ‹network› bude mít tyto metody pro kontrolu (a
 * případnou opravu) své vnitřní struktury:
 *
 *  • ‹has_loops()› – vrátí ‹true› existuje-li v síti cyklus,
 *  • ‹fix_loops()› – rozpojí uzly tak, aby byl výsledek acyklický,
 *    ale pro libovolné uzly, mezi kterými byla před opravou cesta,
 *    musí platit, že po opravě budou nadále vzájemně dosažitelné.
 *
 * Cykly, které prochází více sítěmi (a tedy prohází alespoň dvěma
 * směrovači), neuvažujeme. */
class network;

class node {
    size_t lim = 0;

    void one_way_dc(node *r) {
        neighbors.erase(r);
    }

    bool one_way_con(node *r) {
        if (!has_free_port())
            return false;
        neighbors.insert(r);
        return true;
    }

    bool has_free_port() const {
        return lim != neighbors.size();
    }

    public:
    std::unordered_multiset<node *> neighbors = {};
    network *parent = nullptr;

    node(network *p, size_t len) : lim(len), parent{p}  {}

    virtual bool connect(node *r) {
        if (r == nullptr)
            return false;
        if (r == this && lim - neighbors.size() < 2)
            return false;
        if (!has_free_port())
            return false;
        if (!r->one_way_con(this))
            return false;
        neighbors.insert(r);
        return true;
    }

    [[nodiscard]] virtual bool disconnect(node *r) {
        if (!neighbors.contains(r))
            return false;
        r->one_way_dc(this);
        if (this != r)
            neighbors.erase(r);
        return true;
    }

    bool reachable(const node *r) const {
        if (r == nullptr)
            return false;
        std::unordered_set<const node *> visited = {};
        std::stack<const node *> st = {};
        st.push(this);
        while (!st.empty()) {
            const node *t = st.top();
            st.pop();
            if (t == r)
                return true;
            assert(t != nullptr);
            if (!visited.contains(t))
                visited.insert(t);
            for (const node *x: t->neighbors) {
                assert(x);
                if (!visited.contains(x))
                    st.push(x);
            }
        }
        return false;
    }

    virtual ~node() {
        for (node *n: neighbors) {
            if (n == this)
                continue;
            assert(n->neighbors.contains(this));
            n->neighbors.erase(this);
        }
    }
};

class endpoint : public node {
    public:
    endpoint(network *p) : node(p, 1) {}

    ~endpoint() override = default;
};

class bridge : public node {
    public:
    bridge(network *p, size_t len) : node(p, len) {}

    ~bridge() override = default;
};

class router : public bridge {
    public:
    router(network *p, size_t len) : bridge(p, len) {}

    bool connect(node *r) override {
        if (r == nullptr || r == this)
            return false;
        for (node *n : neighbors) {
            assert(n);
            if (r->parent == n->parent)
                return false;
        }
        return node::connect(r);
    }

    ~router() override = default;
};

class network {
    std::vector<std::unique_ptr<node>> nodes = {};

    bool df_search(const node *source, const node *cur, std::unordered_set<const node *> &visited) const {
        visited.insert(cur);
        for (const node *n: cur->neighbors) {
            if (n == cur)
                return true;
            if (n == source)
                continue;
            if (visited.contains(n))
                return true;
            if (!visited.contains(n) && df_search(cur, n, visited))
                return true;
        }
        return false;
    }

    public:
    node *add_endpoint() {
        nodes.emplace_back(new endpoint(this));
        return nodes.back().get();
    }

    node *add_bridge(size_t len = 2) {
        nodes.emplace_back(new bridge(this, len));
        return nodes.back().get();
    }

    node *add_router(size_t len = 2) {
        nodes.emplace_back(new router(this, len));
        return nodes.back().get();
    }


    bool has_loops() const {
        std::unordered_set<const node *> visited = {};
        for (const auto &n: nodes) {
            if (!visited.contains(n.get()) && df_search(nullptr, n.get(), visited))
                return true;
        }
        return false;
    }

    void fix_loops() {
        std::unordered_set<node *> visited = {};
        std::stack<std::pair<node *, const node*>> st = {};
        std::stack<node *> eraser = {};
        for (auto &n: nodes)
            st.emplace(n.get(), nullptr);
        while (!st.empty()) {
            auto [t, parent] = st.top();
            st.pop();
            if (visited.contains(t))
                continue;
            for (node *n: t->neighbors) {
                if (n == parent)
                    continue;
                if (n != t && !visited.contains(n)) {
                    st.emplace(n, t);
                } else {
                    eraser.push(n);
                }
            }

            while (!eraser.empty()) {
                if (t->neighbors.contains(eraser.top()))
                    assert(t->disconnect(eraser.top()));
                eraser.pop();
            }
        }
    }

};

int main()
{
    network n;
    auto e1 = n.add_endpoint(),
         e2 = n.add_endpoint();
    auto b = n.add_bridge( 2 );
    auto r = n.add_router( 2 );

    assert( e1->connect( b ) );
    assert( b->connect( e2 ) );
    assert( e1->disconnect( b ) );
    assert( !e1->connect( e2 ) );
    assert( e2->reachable( b ) );
    assert( !n.has_loops() );
    n.fix_loops();
    assert( b->reachable( e2 ) );
    assert( r->connect( b ) );
    assert( !r->connect( e1 ) );

    network s1;
    auto b1 = s1.add_bridge();
    auto e21 = s1.add_endpoint();
    assert(!e21->connect(e21));
    assert(b1->connect(b1));
    assert(!b1->connect(e21));
    assert(b1->disconnect(b1));

    auto r1 = s1.add_router();
    assert(!r1->connect(r1));
    auto b2 = s1.add_bridge(4);
    assert(b2->connect(b2));
    assert(b2->connect(b2));
    assert(!b2->connect(e21));
    assert(b2->disconnect(b2));
    assert(!b2->disconnect(b2));
    assert(b2->connect(b2));
    assert(s1.has_loops());
    s1.fix_loops();
    assert(!s1.has_loops());
    /*
  node  2 endpoint 0
  node  4 endpoint 0
  node  5 bridge 1
  node  6 router 1
  node  7 endpoint 1
  connect  1 2
  connect  1 4
  connect  5 6
  connect  5 7
  assert f.reachable( t ) == t.reachable( f ): 11
  assert f.reachable( t ) == expect: 11
  assert f.reachable( t ) == t.reachable( f ): 12
  */

    return 0;
}
