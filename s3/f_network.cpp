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


class network;

const char br = '*';
const char en = '&';
const char ro = '@';

class type {
    public:
    char inner;

    type() : inner('\0') {};

    type(char e) : inner(e) {};

    bool operator==(const char e) const {
        return inner == e;
    }

    bool operator==(const type t) const {
        return inner == t.inner;
    }

    std::string str() const {
        return {inner};
    }

    operator char&() {
        return inner;
    }

    auto operator<=>(const char c) const {
        return inner <=> c;
    }

    auto operator<=>(const type t) const {
        return inner <=> t.inner;
    }
};

using identifier = std::pair<type, std::string>;

class node {
    size_t lim = 0;


    bool has_free_port() const {
        return lim != neighbors.size();
    }

    public:
    identifier id;
    std::map<identifier, node *> neighbors;
    network *parent = nullptr;

    friend class network;

    node(network *p, size_t len) : lim(len), parent{p}  {}

    virtual void one_way_dc(node *r) {
        neighbors.erase(r->id);
    }


    virtual bool one_way_con(node *r) {
        if (!has_free_port()) {
            //std::cout << "onewayfull\n";
            return false;
        }
        neighbors[r->id] = r;
        return true;
    }

    virtual bool connect(node *r) {
        if (r == this) {
            //std::cout << "this" << std::endl;
            return false;
        }
        if (neighbors.contains(r->id)) {
            //std::cout << "contains" << std::endl;
            return false;
        }
        if (!has_free_port()) {
            //std::cout << "full" << std::endl;
            return false;
        }
        if (!r->one_way_con(this))
            return false;
        neighbors[r->id] = r;
        return true;
    }

    [[nodiscard]] virtual bool disconnect(node *r) {
        if (!neighbors.contains(r->id))
            return false;
        r->one_way_dc(this);
        if (this != r)
            neighbors.erase(r->id);
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
            for (const auto &[_, x]: t->neighbors) {
                assert(x);
                if (!visited.contains(x))
                    st.push(x);
            }
        }
        return false;
    }

    virtual ~node() {
        for (auto &[_, n]: neighbors) {
            assert(n->neighbors.contains(id));
            n->neighbors.erase(id);
        }
    }
};

class endpoint : public node {
    public:
    endpoint(network *p, std::size_t n) : node(p, 1) {
        id = {en, std::to_string(n)};
    }

    bool connect(node *r) override {
        if (r == nullptr) {
            //std::cout << "null\n";
            return false;
        }
        if (parent != r->parent) {
            //std::cout << "parent\n";
            return false;
        }
        return node::connect(r);
    }

    ~endpoint() override = default;
};

class bridge : public node {
    public:
    bridge(network *p, size_t len, std::string_view s) : node(p, len) {
        id = {br, s.data()};
    }

    bool connect(node *r) override {
        if (r == nullptr) {
            //std::cout << "null\n";
            return false;
        }
        if (parent != r->parent) {
            //std::cout << "parent\n";
            return false;
        }
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

        if (r->id.first != ro)
            return false;
        for (const auto &[_, n]: neighbors)
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
        id = {ro, s.data()};
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

        if (r->id.first != ro)
            return false;

        for (const auto &[_, n] : neighbors) {
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

using connections = std::map<identifier, std::pair<node *, std::vector<identifier>>>;
using router_connections = std::map<
    std::pair<std::string, std::size_t>, 
    std::pair<router *, std::vector<std::pair<std::string, std::size_t>>>
>;


class network {
    std::vector<std::unique_ptr<node>> nodes;
    static std::size_t ids;
    std::size_t endc = 0;

    std::map<identifier, const node *> to_id_map(const std::vector<const node *> &vec) const {
        std::map<identifier, const node *> res;
        for (const auto &n: vec) {
            res[n->id] = n;
        }
        return res;
    }

    bool df_search(
            const node *source, 
            const node *cur, 
            std::unordered_set<const node *> &visited
        ) const {
        if (cur->id.first == ro)
            return false;

        if (visited.contains(cur))
            return true;
        visited.insert(cur);
        for (const auto &[_, n]: cur->neighbors) {
            if (n == cur)
                return true;
            if (n == source)
                continue;
            if (df_search(cur, n, visited))
                return true;
        }
        return false;
    }

    void find_erace_cycles(
            node *cur,
            node *source,
            std::unordered_set<node *> &visited
            ) {
        std::stack<node *> eraser;

        for (const auto &[_, n]: cur->neighbors)
            if (n != source && visited.contains(n))
                eraser.push(n);

        while (!eraser.empty()) {
            assert(cur->disconnect(eraser.top()));
            eraser.pop();
        }
    }

    void df_fix(
            node *source, 
            node *cur, 
            std::unordered_set<node *> &visited
            ) {
        std::unordered_set<node *> copy = {};
        if (cur->id.first == ro)
            return;

        visited.insert(cur);
        for (const auto &[_, n]: cur->neighbors) {
            if (n != source)
                df_fix(cur, n, visited);
        }
    }
    
    std::vector<node *> nodes_vec(type t) {
        std::vector<node *> res = {};
        for (auto &n: nodes) {
            if (n->id.first == t)
                res.push_back(n.get());
        }
        return res;
    }

    std::vector<const node *> nodes_vec(type t) const {
        std::vector<const node *> res = {};
        for (auto &n: nodes) {
            if (n->id.first == t)
                res.push_back(n.get());
        }
        return res;
    }

    friend std::list<network> deserialize(std::string_view);
    friend void connect_in_net(network &, connections &);

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
            if (n->id.first == ro) {
                add_router(n->lim, n->id.second);
            } else if (n->id.first == en) {
                add_endpoint();
            } else {
                add_bridge(n->lim, n->id.second);
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


    std::vector<node *> endpoints() {
        return nodes_vec(en);
    }

    std::vector<node *> bridges() {
        return nodes_vec(br);
    }

    std::vector<node *> routers() {
        return nodes_vec(ro);
    }

    std::vector<const node *> endpoints() const {
        return nodes_vec(en);
    }

    std::vector<const node *> bridges() const {
        return nodes_vec(br);
    }

    std::vector<const node *> routers() const {
        return nodes_vec(ro);
    }

    int size() const {
        return nodes.size();
    }

    std::ostream &serialize_ends(std::ostream &os) const {
        auto ends = endpoints();
        os << en << " " << ends.size() << " ";
        std::size_t in_en_count = 0;
        for (const auto &e: ends) {
            if (!e->neighbors.empty() && e->neighbors.begin()->first.first == en)
                ++in_en_count;
        }
        os << in_en_count << std::endl;
        return os;
    }

    std::ostream &serialize_bridges(std::ostream &os) const {
        for (const auto &[id, b]: to_id_map(bridges())) {
            os << "*" << id.second << " " << b->lim << " ";
            std::ostringstream neighbors;
            std::size_t endc = 0;
            for (const auto &[nid, n]: b->neighbors) {
                if (nid.first == en) {
                    ++endc;
                    continue;
                }
                neighbors << nid.second  + nid.first.str();
                neighbors << " ";
            }
            os << endc << " " << neighbors.str() << "\n";
        }
        return os;
    }

    std::ostream &serialize_routers(std::ostream &os, const std::map<std::size_t, std::size_t> &ids) const {
        for (const auto &[id, r]: to_id_map(routers())) {
            os << "@" << r->id.second << " " << r->lim << " ";
            std::ostringstream neighbors = {};
            std::string in_net = "%";
            for (const auto &[nid, n]: r->neighbors) {
                [[unlikely]] if (nid.first != ro) {
                    in_net = "";
                    if (nid.first == br)
                        in_net += nid.second;
                    in_net += nid.first.str();
                    continue;
                }
                assert(n->parent);
                neighbors << ids.at(n->parent->id) << " ";
                neighbors << nid.second << " ";
            }
            os << in_net << " " << neighbors.str() << "\n";
        }
        return os;

    }


    std::ostream &serialize(std::ostream &os, const std::map<std::size_t, std::size_t> &ids) const {
        os << "#" << ids.at(id) << "\n";
        serialize_ends(os);
        serialize_bridges(os);
        serialize_routers(os, ids);
        return os;
    }

};

network make_network(std::size_t id) {
    network res;
    res.id = id;
    return res;
}

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
    //std::cout << res.str() << std::endl;
    //std::cout << "-------------------" << std::endl;
    return res.str();
};


void connect_in_net(network &net, connections &cons) {
    for (auto &n: net.nodes) {
        for (const auto &neigh: cons[n->id].second) {
            n->connect(cons[neigh].first);
        }
    }
}

void deserialize_net(
        std::istringstream &line, 
        connections &net_connections,
        std::size_t &net_id,
        std::map<std::size_t, network> &subres
        ) {
    line >> net_id;
    net_connections = {};
    subres[net_id] = make_network(net_id);
}

void deserialize_end(
        std::istringstream &line, 
        network &net,
        std::size_t &endi
        ) {
    std::size_t endc = 0;
    line >> endc;
    line >> endi;
    assert(endi % 2 == 0);
    node *cur = nullptr, *prev = nullptr;
    for (std::size_t i = 0; i < endi; ++i) {
        if (i % 2) {
            cur = net.add_endpoint();
            assert(cur->connect(prev));
        } else {
            prev = net.add_endpoint();
        }
    }

    for (std::size_t i = endi; i < endc; ++i)
        net.add_endpoint();
}

void deserialize_bridge(
        std::istringstream &line, 
        connections &net_connections,
        identifier &id,
        network &net,
        std::size_t &endi
        ) {
    std::size_t lim;
    type t;
    line >> id.second;
    assert(!id.second.empty());
    line >> lim;
    std::size_t endc = 0;
    line >> endc;
    std::vector<node *> ends;
    if (endc) {
        ends = net.endpoints();
    }
    node *br = net.add_bridge(lim, id.second);
    net_connections[id] = {br, {}};
    for (std::size_t i = 0; i < endc; ++i) {
        assert(endi < ends.size());
        assert(br->connect(ends[endi++]));
    }
    while (!line.eof()) {
        std::string neigh_id;
        line >> neigh_id;
        if (neigh_id.empty())
            continue;
        t = neigh_id.back();
        neigh_id.pop_back();
        net_connections[id].second.emplace_back(t, neigh_id);
    }
}

void deserialize_rout(
        std::istringstream &line, 
        connections &net_connections,
        network &net,
        router_connections &routers,
        std::size_t &endi
        ) {
    std::size_t lim;
    type t;
    std::string in_net;
    std::string id;
    line >> id;
    assert(!id.empty());
    line >> lim;
    line >> in_net;
    const std::pair<std::string &, std::size_t> key = {id, net.id};
    auto rout = dynamic_cast<router *>(net.add_router(lim, id));
    routers[key] = {rout, {}};
    auto neighs = &routers[key].second;
    if (in_net != "%") {
        t = in_net.back();
        in_net.pop_back();
        if (t == en) {
            rout->connect(net.endpoints()[endi++]);
        } else {
            rout->connect(net_connections[{t, in_net}].first);
        }
    }
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

void connect_routers(router_connections &routers) {
    for (auto &[rid, n]: routers) {
        auto &[router, neighs] = n;
        for (const auto &neigh: neighs) {
            assert(routers.contains(neigh));
            if (!router->neighbors.contains(routers[neigh].first->id)) {
                assert(router->connect(routers[neigh].first));
            }
        }
    }
}


std::list< network > deserialize( std::string_view s) {
    std::istringstream in(s.data());
    std::map<std::size_t, network> subres;
    connections net_connections;
    router_connections routers;
    std::size_t net_id = 0;
    identifier id;
    std::size_t endi = 0;
    while (!in.eof()) {
        std::string aux;
        std::getline(in, aux);
        std::istringstream line(aux);
        id.first = line.get();
        switch (id.first) {
            case '#':   connect_in_net(subres[net_id], net_connections);
                        deserialize_net(line, net_connections, net_id, subres);
                        break;
            case br:    deserialize_bridge(line, net_connections, id, subres[net_id], endi); break;
            case ro:    deserialize_rout(line, net_connections, subres[net_id], routers, endi); break;
            case en:    deserialize_end(line, subres[net_id], endi);
        }
    }
    connect_in_net(subres[net_id], net_connections);
    connect_routers(routers);
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

    std::list<network> ver_perm{2};
    e1 = ver_perm.back().add_endpoint();
    e2 = ver_perm.back().add_endpoint();
    r1 = ver_perm.back().add_router(3, "A");
    e3 = ver_perm.back().add_endpoint();
    assert(e1->connect(e3));
    assert(e2->connect(r1));

    str = serialize(ver_perm);

    std::list<network> ver_h {};
    ver_h.resize(ver_perm.size());
    e2 = ver_h.back().add_endpoint();
    e1 = ver_h.back().add_endpoint();
    r1 = ver_h.back().add_router(3, "A");
    e3 = ver_h.back().add_endpoint();
    assert(r1->connect(e2));
    assert(e3->connect(e1));

    assert(str == serialize(ver_h));



    /*  node  i → 1, ni.type → endpoint, ni.network → 1
  net_id → 1, node_id → 2, capacity → 1
  node = net->add_endpoint() ; 
  net_id → 1, node_id → 1, capacity → 1
  node = net->add_endpoint() ; 
  net_id → 1, node_id → 3, capacity → 3
  node = net->add_router( capacity, ni.idstr ) ; 
  ni.connect( g.at( other_id ) ) ;  3 2
  net_id → 1, node_id → 4, capacity → 1
  node = net->add_endpoint() ; 
  ni.connect( g.at( other_id ) ) ;  4 1
  auto str_2 { _student_::serialize( h ) → "#0\n#1\n&0 A@\n&1 2&\n&2 1&\n@A 3 0\
  & \n" }
  assert str_1 == str_2
  eval/f_permute.t.cpp:75: str_1 == str_2
    str_1 = "#0\n#1\n&0 2&\n&1 A@\n&2 0&\n@A 3 1& \n"
    str_2 = "#0\n#1\n&0 A@\n&1 2&\n&2 1&\n@A 3 0& \n"*/
    
    std::list<network> ver2_perm1{2};
    b1 = ver2_perm1.back().add_bridge(2, "A");
    b2 = ver2_perm1.back().add_bridge(2, "B");
    auto b3 = ver2_perm1.back().add_bridge(2, "C");
    b1->connect(b2);
    b1->connect(b3);
    b2->connect(b3);
    str = serialize(ver2_perm1);

    std::list<network> ver2_perm2{2};
    b2 = ver2_perm2.back().add_bridge(2, "B");
    b1 = ver2_perm2.back().add_bridge(2, "A");
    b1->connect(b2);
    b3 = ver2_perm2.back().add_bridge(2, "C");
    b3->connect(b1);
    b3->connect(b2);
    assert(serialize(ver2_perm2) == str);
    /*
     *
  std::list< _student_::network > h { }
  h.resize( g.networks.size() ) ; 
  net_id → 1, node_id → 2, capacity → 2
  node = net->add_bridge( capacity, ni.idstr ) ; 
  net_id → 1, node_id → 1, capacity → 2
  node = net->add_bridge( capacity, ni.idstr ) ; 
  ni.connect( g.at( other_id ) ) ;  1 2
  net_id → 1, node_id → 3, capacity → 2
  node = net->add_bridge( capacity, ni.idstr ) ; 
  ni.connect( g.at( other_id ) ) ;  3 1
  ni.connect( g.at( other_id ) ) ;  3 2
  auto str_2 { _student_::serialize( h ) → "#0\n& 0 0\n#1\n& 0 0\n*B 2 0 A* C* \
  \n*A 2 0 B* C* \n*C 2 0 A* B* \n" }
  assert str_1 == str_2
  eval/f_permute.t.cpp:75: str_1 == str_2
    str_1 = "#0\n& 0 0\n#1\n& 0 0\n*A 2 0 B* C* \n*B 2 0 A* C* \n*C 2 0 A* B* \n"
    str_2 = "#0\n& 0 0\n#1\n& 0 0\n*B 2 0 A* C* \n*A 2 0 B* C* \n*C 2 0 A* B* \n"*/

    return 0;
}
