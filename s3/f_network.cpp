#include <cassert>
#include <sstream>
#include <ostream>
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
#include <cstdio>
#include <cstring>
#include <map>
#include <set>

class network;

class node {
    size_t lim = 0;


    bool has_free_port() const {
        return lim != neighbors.size();
    }

    public:
    bool is_router = false;
    std::string id;
    std::set<node *> neighbors;
    network *parent = nullptr;

    friend class network;

    node(network *p, size_t len) : lim(len), parent{p}  {}

    virtual void one_way_dc(node *r) {
        neighbors.erase(r);
    }


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
            for (const auto &x: t->neighbors) {
                assert(x);
                if (!visited.contains(x))
                    st.push(x);
            }
        }
        return false;
    }

    virtual ~node() {
        for (auto &n: neighbors) {
            assert(n->neighbors.contains(this));
            n->neighbors.erase(this);
        }
    }
};

class endpoint : public node {
    public:
    endpoint(network *p, std::size_t i) : node(p, 1) {
        is_router = false;
        std::ostringstream aux;
        aux << i;
        aux << "$";
        id = aux.str();
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
        for (const auto &n: neighbors)
            if (n->parent == r->parent)
                return false;
        return node::one_way_con(r);
    }

    void one_way_dc(node *n) noexcept override {
        if (n->parent == parent)
            has_in_network = false;
        node::one_way_dc(n);
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

        for (const auto &n : neighbors) {
            if (r->parent == n->parent)
                return false;
        }
        return node::connect(r);
    }

    bool disconnect(node *n) noexcept override {
        if (node::disconnect(n)) {
            if (n->parent == parent)
                has_in_network = false;
            return true;
        }
        return false;
    }

    ~router() override = default;
};

using connections = std::map<std::string, std::pair<node *, std::vector<std::string>>>;

class network {
    std::vector<std::unique_ptr<node>> nodes;
    static std::size_t ids;
    std::size_t endc = 0;

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
        for (const auto &n: cur->neighbors) {
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

        visited.insert(cur);
        std::stack<node *> eraser;

        for (const auto &n: cur->neighbors)
            if (n != source && visited.contains(n))
                eraser.push(n);

        while (!eraser.empty()) {
            assert(cur->disconnect(eraser.top()));
            eraser.pop();
        }

        for (const auto &n: cur->neighbors) {
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

    friend std::list<network> deserialize(std::string_view);
    friend void connect_all(network &, connections &);

    template<typename T>
    friend std::vector<std::size_t> reachable_for(const T &, const network &);

    public:
    std::size_t id;
    network() : id{ids++} {}
    network(network &&n) : id{n.id} {
        nodes = std::move(n.nodes);
        for (auto &n: nodes) {
            n->parent = this;
        }
    }

    network(const network &n) : id{ids++} {
        for (auto &n: n.nodes) {
            if (n->is_router) {
                add_router(n->lim, n->id);
            } else if (n->id.back() == '$') {
                add_endpoint();
            } else {
                add_bridge(n->lim, n->id);
            }
        }
    }

    network &operator=(network &&n) {
        id = n.id;
        nodes = std::move(n.nodes);
        return *this;
    }

    node *add_endpoint() {
        nodes.emplace_back(new endpoint(this, endc++));
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

    std::ostream &serialize(std::ostream &os, std::map<std::size_t, std::size_t> ids) const {
        os << "#" << ids[id] << "\n";
        for (const auto &e: endpoints()) {
            os << "&" << e->id << " " << (e->neighbors.empty() ? "%" : (*e->neighbors.begin())->id) << "\n" ;
        }
        for (const auto &b: bridges()) {
            os << "*" << b->id << " " << b->lim << " ";
            std::ostringstream neighbors;
            for (const auto &n: b->neighbors) {
                neighbors << n->id;
                neighbors << " ";
            }
            os << neighbors.str() << "\n";
        }
        for (const auto &r: routers()) {
            os << "@" << r->id << " " << r->lim << " ";
            std::ostringstream neighbors = {};
            std::string in_net = "%";
            for (const auto &n: r->neighbors) {
                [[unlikely]] if (!n->is_router) {
                    in_net = n->id;
                    continue;
                }
                assert(n->parent);
                neighbors << ids[n->parent->id] << " ";
                neighbors << n->id << " ";
            }
            os << in_net << " " << neighbors.str() << "\n";
        }
        return os;
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

/*data budu mat nasledovnu formu:
#{network id} {endpoint count}
\t{bridge id} {lim} {endpoint count} {sused 1 id} {sused 2 id} ...
\t ...
\v{router id} {lim} {vnutro sietovy sused} {sused 1 siet id} {sused 1 id}...
\v...
#...
*/
//# znaci novu siet, \t novy bridge v poslednej sieti, \v novy router

std::map<std::size_t, std::size_t> get_ids(const std::list<network> &l) {
    std::map<std::size_t, std::size_t> res;
    std::size_t id = 0;
    for (const auto &n: l) {
        res[n.id] = id++;
    }
    return res;
}

std::string serialize( const std::list< network > & l) {
    std::ostringstream res;
    auto ids = get_ids(l);
    for (const auto &n: l) {
        n.serialize(res, ids);
    }
    return res.str();
};


void connect_all(network &net, connections &cons) {
    for (auto &n: net.nodes) {
        for (const auto &neigh: cons[n->id].second) {
            n->connect(cons[neigh].first);
        }
    }
}

std::list< network > deserialize( std::string_view s) {
    std::istringstream in(s.data());
    std::map<std::size_t, network> subres;
    std::map<std::string, endpoint *> ends;
    connections net_connections;
    std::map<
        std::pair<std::string, std::size_t>, 
        std::pair<router *, std::vector<std::pair<std::string, std::size_t>>>
            > routers;
    std::size_t endc = 0;
    std::size_t lim = 0;
    std::size_t net_id = 0;
    std::string id;
    while (!in.eof()) {
        std::string aux;
        std::getline(in, aux);
        std::istringstream line(aux);
        char mark = line.get();
        switch (mark) {
            case '#': {
                          connect_all(subres[net_id], net_connections);
                          line >> net_id;
                          line >> endc;
                          net_connections = {};
                          subres[net_id] = network();
                          for (std::size_t i = 0; i < endc; ++i) {
                              auto aux = subres[net_id].add_endpoint();
                              net_connections[aux->id] = {aux, {}};
                          }
                          break;
                      }

            case '&': {
                          line >> id;
                          net_connections[id] = {subres[net_id].add_endpoint(), {}};
                          line >> aux;
                          if (aux != "%") {
                              net_connections[id].second.push_back(aux);
                          }
                          break;
                      }

            case '*': {
                           line >> id;
                           assert(!id.empty());
                           line >> lim;
                           net_connections[id] = {subres[net_id].add_bridge(lim, id), {}};
                           while (!line.eof()) {
                               std::string neigh_id;
                               line >> neigh_id;
                               if (neigh_id.empty())
                                   continue;
                               net_connections[id].second.emplace_back(neigh_id);
                           }
                           break;
                       }
            case '@': {
                           std::string in_net;
                           line >> id;
                           assert(!id.empty());
                           line >> lim;
                           line >> in_net;
                           const std::pair<std::string &, std::size_t> key = {id, net_id};
                           routers[key] = {dynamic_cast<router *>(subres[net_id].add_router(lim, id)), {}};
                           auto rout = routers[key].first;
                           auto neighs = &routers[key].second;
                           if (in_net != "%")
                               rout->connect(net_connections[in_net].first);
                           while (!line.eof()) {
                               std::string neigh_id;
                               std::size_t par;
                               line >> par;
                               line >> neigh_id;
                               if (neigh_id.empty())
                                   continue;
                               neighs->push_back({neigh_id, par});
                           }
                       }
        }
    }
    connect_all(subres[net_id], net_connections);
    for (auto &[rid, n]: routers) {
        auto &[router, neighs] = n;
        for (const auto &neigh: neighs) {
            assert(routers.contains(neigh));
            if (!router->neighbors.contains(routers[neigh].first)) {
                assert(router->connect(routers[neigh].first));
            }
        }
    }
    std::list<network> res = {};

    for (auto &sub: subres) {
        res.push_back(std::move(sub.second));
    }
    return res;
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

template <typename T>
std::vector<std::size_t> reachable_for(const T &ns, const network &net) {
    std::vector<std::size_t> res(ns.size());
    for (std::size_t i = 0; i < ns.size(); ++i) {
        for (const auto &aux: net.nodes) {
            if (ns[i]->reachable(aux.get())) {
                res[i]++;
            }
        }
    }
    return res;
}

void print(std::size_t i) {
    std::cout << i;
}


void print(const auto &vec) {
    std::cout << "[ ";
    for (const auto &sub: vec) {
        print(sub);
        std::cout << ", ";
    }
    std::cout << " ]";
}

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

    network disc_san1;
    auto b1 = disc_san1.add_bridge(2, "A");
    e1 = disc_san1.add_endpoint();
    e2 = disc_san1.add_endpoint();
    network disc_san2;
    auto b2 = disc_san2.add_bridge(2, "A");
    r1 = disc_san2.add_router(2, "A");
    e3 = disc_san2.add_endpoint();

    assert(b1->connect(e1));
    assert(b1->connect(e2));
    assert(b2->connect(r1));
    assert(b2->connect(e3));

    str = serialize({disc_san1, disc_san2});
    assert(b1->disconnect(e1));
    assert(e1->connect(b1));
    assert(str == serialize({disc_san1, disc_san2}));

    assert(b1->disconnect(e2));
    assert(e2->connect(b1));
    assert(str == serialize({disc_san1, disc_san2}));
    
    assert(b2->disconnect(e3));
    assert(e3->connect(b2));
    assert(str == serialize({disc_san1, disc_san2}));

    assert(b2->disconnect(r1));
    assert(r1->connect(b2));
    assert(str == serialize({disc_san1, disc_san2}));

    std::list<network> perm1;
    perm1.emplace_front();
    perm1.emplace_front();
    e1 = perm1.back().add_endpoint();
    e2 = perm1.back().add_endpoint();
    assert(e1->connect(e2));
    str = serialize(perm1);
    std::list<network> h{perm1.size()}; 
    e3 = h.back().add_endpoint();
    auto e4 = h.back().add_endpoint();
    assert(e4->connect(e3));

    /*
     *   node  1 bridge 0
  node  2 endpoint 0
  node  3 router 0
  node  4 endpoint 1
  node  5 endpoint 1
  connect  1 2
  connect  1 3
  connect  4 5*/
    std::list<network> reach;
    reach.emplace_front();
    reach.emplace_front();
    b1 = reach.front().add_bridge(2, "A");
    e1 = reach.front().add_endpoint();
    r1 = reach.front().add_router(3, "A");
    e2 = reach.back().add_endpoint();
    e3 = reach.back().add_endpoint();
    assert(b1->connect(e1));
    assert(b1->connect(r1));
    assert(e2->connect(e3));
    std::vector<std::vector<std::size_t>> out;
    std::vector<std::vector<std::vector<std::size_t>>> counts1;
    out.push_back(reachable_for(reach.front().endpoints(), reach.front()));
    out.push_back(reachable_for(reach.front().bridges(), reach.front()));
    out.push_back(reachable_for(reach.front().routers(), reach.front()));
    counts1.push_back(std::move(out));
    out = {};
    out.push_back(reachable_for(reach.back().endpoints(), reach.back()));
    out.push_back(reachable_for(reach.back().bridges(), reach.back()));
    out.push_back(reachable_for(reach.back().routers(), reach.back()));
    counts1.push_back(std::move(out));
    out = {};

    str = serialize(reach);
    reach = deserialize(str);

    std::vector<std::vector<std::vector<std::size_t>>> counts2;
    out.push_back(reachable_for(reach.front().endpoints(), reach.front()));
    out.push_back(reachable_for(reach.front().bridges(), reach.front()));
    out.push_back(reachable_for(reach.front().routers(), reach.front()));
    counts2.push_back(std::move(out));
    out = {};
    out.push_back(reachable_for(reach.back().endpoints(), reach.back()));
    out.push_back(reachable_for(reach.back().bridges(), reach.back()));
    out.push_back(reachable_for(reach.back().routers(), reach.back()));
    counts2.push_back(std::move(out));
    out = {};
    assert(reach.back().endpoints()[0]->reachable(reach.back().endpoints()[1]));
    assert(counts2 == counts1);

    network ver_disc;
    b1 = ver_disc.add_bridge(2, "A");
    b2 = ver_disc.add_bridge(2, "B");
    r1 = ver_disc.add_router(3, "A");
    e1 = ver_disc.add_endpoint();
    assert(b1->connect(b2));
    assert(b1->connect(e1));
    assert(!b2->connect(b1));
    assert(b2->connect(r1));
    
    /**/

    return 0;
}
