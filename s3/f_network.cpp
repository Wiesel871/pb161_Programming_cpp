#include <cassert>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <cstddef>
#include <cstdio>
#include <stack>
#include <vector>
#include <memory>
#include <unordered_set>
#include <list>
#include <sstream>
#include <iostream>

class network;

class node {
    size_t lim = 0;

    void one_way_dc(node *r) {
        neighbors.erase(r);
    }


    bool has_free_port() const {
        return lim != neighbors.size();
    }

    public:
    bool is_router = false;
    std::string id;
    std::unordered_set<node *> neighbors = {};
    network *parent = nullptr;

    node(network *p, size_t len) : lim(len), parent{p}  {}

    virtual bool one_way_con(node *r) {
        if (!has_free_port())
            return false;
        neighbors.insert(r);
        return true;
    }

    virtual bool connect(node *r) {
        if (r == this)
            return false;
        if (neighbors.contains(r))
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
    endpoint(network *p) : node(p, 1) {
        is_router = false;
        id = "";
    }

    bool connect(node *r) override {
        if (r == nullptr)
            return false;
        if (parent != r->parent)
            return false;
        return node::connect(r);
    }

    ~endpoint() override = default;
};

class bridge : public node {
    public:
    bridge(network *p, size_t len, std::string_view s) : node(p, len) {
        is_router = false;
        id = s;
    }

    bool connect(node *r) override {
        if (r == nullptr)
            return false;
        if (parent != r->parent)
            return false;
        return node::connect(r);
    }

    ~bridge() override = default;
};

class router : public node {
    bool has_in_network = false;

    bool one_way_con(node *r) override {
        if (parent == r->parent) {
            if (has_in_network)
                return false;
            if (node::one_way_con(r)) {
                has_in_network = true;
                return true;
            }
            return false;
        }

        if (!r->is_router)
            return false;
        for (const node *n: neighbors)
            if (n->parent == r->parent)
                return false;
        return node::one_way_con(r);
    }

    public:
    router(network *p, size_t len, std::string_view s) : node(p, len) {
        is_router = true;
        id = s;
    }

    bool connect(node *r) override {
        if (r == nullptr || r == this) {
            return false;
        }
        if (parent == r->parent) {
            if (has_in_network) 
                return false;
            if (node::connect(r)) {
                has_in_network = true;
                return true;
            }
            return false;
        }

        if (!r->is_router)
            return false;

        for (const node *n : neighbors) {
            if (r->parent == n->parent)
                return false;
        }
        return node::connect(r);
    }

    ~router() override = default;
};

class network {
    std::vector<std::unique_ptr<node>> nodes = {};
    static std::size_t ids;

    bool df_search(
            const node *source, 
            const node *cur, 
            std::unordered_set<const node *> &visited
        ) const {
        if (cur->is_router)
            return false;

        if (visited.contains(cur))
            return true;
        visited.insert(cur);
        for (const node *n: cur->neighbors) {
            if (n == cur)
                return true;
            if (n == source)
                continue;
            if (df_search(cur, n, visited))
                return true;
        }
        return false;
    }

    void df_fix(
            node *source, 
            node *cur, 
            std::unordered_set<node *> &visited
            ) {
        std::unordered_set<node *> copy = {};
        if (cur->is_router)
            return;

        if (cur->neighbors.contains(cur))
            cur->neighbors.erase(cur);

        visited.insert(cur);
        std::stack<node *> eraser = {};

        for (auto n: cur->neighbors)
            if (n != source && visited.contains(n))
                eraser.push(n);

        while (!eraser.empty()) {
            assert(cur->disconnect(eraser.top()));
            eraser.pop();
        }

        for (auto n: cur->neighbors) {
            if (n != source)
                df_fix(cur, n, visited);
        }
    }
    
    template<class T>
    std::vector<T> nodes_vec() {
        std::vector<T> res = {};
        for (auto &n: nodes) {
            if (auto p = dynamic_cast<T>(n.get()); p != nullptr) {
                res.push_back(p);
            }
        }
        return res;
    }

    template<class T>
    std::vector<T> nodes_vec() const {
        std::vector<T> res = {};
        for (auto &n: nodes) {
            if (auto p = dynamic_cast<T>(n.get()); p != nullptr) {
                res.push_back(p);
            }
        }
        return res;
    }

    public:
    const std::size_t id;
    network() : id{ids++} {}

    node *add_endpoint() {
        nodes.emplace_back(new endpoint(this));
        return nodes.back().get();
    }

    node *add_bridge(size_t len, std::string_view s) {
        nodes.emplace_back(new bridge(this, len, s));
        return nodes.back().get();
    }

    node *add_router(size_t len, std::string_view s) {
        nodes.emplace_back(new router(this, len, s));
        return nodes.back().get();
    }


    bool has_loops() const {
        std::unordered_set<const node *> visited = {};
        for (const auto &n: nodes) {
            if (!visited.contains(n.get()) && df_search(nullptr, n.get(), visited)) {
                return true;
            }
        }
        return false;
    }

    void fix_loops() {
        std::unordered_set<node *> visited = {};
        for (auto &n: nodes) {
            if (!visited.contains(n.get())) {
                df_fix(nullptr, n.get(), visited);
            }
        }
    }


    std::vector<endpoint *> endpoints() {
        return nodes_vec<endpoint *>();
    }

    std::vector<bridge *> bridges() {
        return nodes_vec<bridge *>();
    }

    std::vector<router *> routers() {
        return nodes_vec<router *>();
    }

    std::vector<const endpoint *> endpoints() const {
        return nodes_vec<const endpoint *>();
    }

    std::vector<const bridge *> bridges() const {
        return nodes_vec<const bridge *>();
    }

    std::vector<const router *> routers() const {
        return nodes_vec<const router *>();
    }

    int size() const {
        return nodes.size();
    }

    std::string serialize() const {
        std::ostringstream res;
        res << "#" << id << " " << endpoints().size() << "\n";
        for (const auto &b: bridges()) {
            res << "\t" << b->id;
            std::string neighbors;
            std::size_t endpoint_count = 0;
            for (const auto &n: b->neighbors) {
                if (b->id.empty()) {
                    ++endpoint_count;
                    continue;
                }
                neighbors += n->id + " ";
            }
            res << " " << endpoint_count << " " << neighbors << "\n";
        }
        for (const auto &r: routers()) {
            res << "\v" << r->id;
            std::ostringstream neighbors;
            std::string in_net = "0";
            for (const auto &n: r->neighbors) {
                [[unlikely]] if (!n->is_router) {
                    in_net = n->id.empty() ? "e" : n->id;
                    continue;
                }
                neighbors << n->parent->id << " " << n->id << " ";
            }
            res << " " << in_net << " " << neighbors.str() << "\n";
        }
        return res.str();
    }

};

std::size_t network::ids = 0;

/* Navrhněte textový formát pro ukládání informací o sítích tak, jak
 * byly definované v příkladu ‹s2/e_network›, který bude mít tyto
 * vlastnosti:
 *
 *  • do jednoho řetězce musí být možno uložit libovolný počet sítí,
 *    které mohou být vzájemně propojené směrovači,
 *  • výsledek načtení z řetězce nesmí být možné odlišit (použitím
 *    veřejného rozhraní) od hodnot, kterých uložením řetězec
 *    vznikl,
 *  • obsah řetězce musí být plně určen aktuálním stavem vstupních
 *    sítí, bez ohledu na posloupnost operací, kterými vznikly –
 *    zejména tedy nezmí záležet na pořadí přidávání a propojování
 *    (případně rozpojování) uzlů,¹
 *  • jako speciální případ předchozího, načtení a následovné
 *    uložení sítě musí být idempotentní (výsledný řetězec je
 *    identický jako ten, se kterým jsme začali).
 *
 * Rozhraní je dané těmito dvěma čistými funkcemi (zejména se žádná
 * síť nesmí změnit použitím funkce ‹serialize›): */

std::string serialize( const std::list< network > & l) {
    std::string res;
    for (const auto &n: l) {
        res += n.serialize();
    }
    std::cout << res << std::endl;
    return res;
};

std::list< network > deserialize( std::string_view s) {
    std::istringstream i(s.data());
    std::vector<network> res;
    std::string id;
    std::size_t ends = 0;
    while (!i.eof()) {
        char mark = i.get();
        if (mark == '#') {
            std::size_t aux = 0;
            i >> aux;
            i >> ends;
            printf("%c %lu %lu\n", mark, aux, ends);
        } else if (mark == '\t') {
            i >> id;
            i >> ends;
            printf("%c | \'%s\' | %lu\n", mark, id.data(), ends);
            while (i.peek() == ' ') {
                std::string aux;
                i >> aux;
                printf("%c | \'%s\' | \'%s\'\n", mark, id.data(), aux.data());

            }
        }
    }
    return {res.begin(), res.end()};
};

/* Aby se Vám serializace snáze implementovala, přidejte metodám
 * ‹add_bridge› a ‹add_router› parametr typu ‹std::string_view›,
 * který reprezentuje neprázdný identifikátor sestavený z číslic a
 * anglických písmen. Identifikátor je unikátní pro danou síť a typ
 * uzlu.
 *
 * Konečně aby bylo možné s načtenou sítí pracovat, přidejte metody
 * ‹endpoints›, ‹bridges› a ‹routers›, které vrátí vždy
 * ‹std::vector› ukazatelů vhodného typu. Konkrétní pořadí uzlů
 * v seznamu není určeno. */

/* ¹ Samozřejmě záleží na pořadí, ve kterém jsou sítě předány
 *   serializační funkci – serializace sítí ‹a, b› se může obecně
 *   lišit od serializace ‹b, a›. */

int main()
{
    std::list< network > sys_1;

    auto &n = sys_1.emplace_back();
    auto &m = sys_1.emplace_back();

    auto e1 = n.add_endpoint(),
         e2 = n.add_endpoint(),
         e3 = m.add_endpoint();
    auto b = n.add_bridge( 2, "ufo" );
    auto r1 = n.add_router( 2, "r1" );
    auto r2 = m.add_router( 2, "r2" );

    assert( n.bridges().size() == 1 );
    assert( n.routers().size() == 1 );
    assert( n.endpoints().size() == 2 );

    assert( b->connect( e1 ) );
    assert( b->connect( r1 ) );
    assert( r1->connect( r2 ) );
    assert( r2->connect( e3 ) );

    assert( e1->reachable( e3 ) );
    assert( !e1->reachable( e2 ) );

    std::string str = serialize( sys_1 );
    std::list< network > sys_2 = deserialize( str );
    assert( sys_2.size() == 2 );
    assert( serialize( sys_2 ) == str );

    const network &nn = sys_2.front(),
                  &mm = sys_2.back();

    auto nn_e = nn.endpoints();
    auto mm_e = mm.endpoints();

    assert( nn_e.size() == 2 );
    assert( mm_e.size() == 1 );

    assert( nn_e.front()->reachable( mm_e.front() ) ||
            nn_e.back()->reachable( mm_e.front() ) );

    return 0;
}
