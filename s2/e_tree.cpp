#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <new>
#include <stdexcept>
#include <vector>
#include <memory>
#include <map>

/* Uvažujme stromovou strukturu, která má 4 typy uzlů, a která
 * představuje zjednodušený JSON:
 *
 *  • ‹node_bool› – listy typu ‹bool›,
 *  • ‹node_int› – listy typu ‹int›,
 *  • ‹node_array› – indexované celými čísly souvisle od nuly,
 *  • ‹node_object› – klíčované libovolnými celými čísly.
 *
 * Typ ‹tree› pak reprezentuje libovolný takový strom (včetně
 * prázdného a jednoprvkového). Pro hodnoty ‹t› typu ‹tree›, ‹n›
 * libovolného výše uvedeného typu ‹node_X› a ‹idx› typu ‹int›, jsou
 * všechny níže uvedené výrazy dobře utvořené.
 *
 * Práce s hodnotami typu ‹tree›:
 *
 *  • ‹t.is_null()› – vrátí ‹true› reprezentuje-li tato hodnota
 *    «prázdný strom»,
 *  • ‹*t› – platí-li ‹!t.is_null()›, jsou ‹(*t)› a ‹n› záměnné,
 *    jinak není definováno,
 *  • implicitně sestrojená hodnota reprezentuje prázdný strom,
 *  • hodnoty typu ‹tree› lze také vytvořit volnými funkcemi
 *    ‹make_X›, kde výsledkem je vždy strom s jediným uzlem typu
 *    ‹node_X› (v případě ‹make_bool› resp. ‹make_int› s hodnotou
 *    ‹false› resp. ‹0›, není-li v parametru uvedeno jinak).
 *
 * Hodnoty typu ‹node_X› lze sestrojit implicitně, a reprezentují
 * ‹false›, ‹0› nebo prázdné pole (objekt).
 *
 * Skalární operace (výsledkem je zabudovaný skalární typ):
 *
 *  • ‹n.is_X()› – vrátí ‹true›, je-li typ daného uzlu ‹node_X›
 *    (např. ‹is_bool()› určí, je-li uzel typu ‹node_bool›),
 *  • ‹n.size()› vrátí počet potomků daného uzlu (pro listy 0),
 *  • ‹n.as_bool()› vrátí ‹true› je-li ‹n› uzel typu ‹node_bool› a
 *    má hodnotu ‹true›, nebo je to uzel typu ‹node_int› a má
 *    nenulovou hodnotu, nebo je to neprázdný uzel typu ‹node_array›
 *    nebo ‹node_object›,
 *  • ‹n.as_int()› vrátí 1 nebo 0 pro uzel typu ‹node_bool›, hodnotu
 *    uloženou n uzlu ‹node_int›, nebo skončí výjimkou
 *    ‹std::domain_error›.
 *
 * Operace přístupu k potomkům:
 *
 *  • ‹n.get( idx )› vrátí odkaz (referenci) na potomka:
 *    ◦ s indexem (klíčem) ‹idx› vhodného typu tak, aby
 *      s ní bylo možné pracovat jako s libovolnou hodnotou typu
 *      ‹node_X›, nebo
 *    ◦ skončí výjimkou ‹std::out_of_range› když takový potomek
 *      neexistuje,
 *  • ‹n.copy( idx )› vrátí potomka na indexu (s klíčem) ‹idx› jako
 *    «hodnotu» typu ‹tree›, nebo skončí výjimkou
 *    ‹std::out_of_range› neexistuje-li takový.
 *
 * Operace, které upravují existující strom:
 *
 *  • ‹n.set( idx, t )› nastaví potomka na indexu nebo u klíče ‹i› na
 *    hodnotu ‹t›, přitom samotné ‹t› není nijak dotčeno, přitom:
 *    ◦ je-li ‹n› typu ‹node_array›, je rozšířeno dle potřeby tak,
 *      aby byl ‹idx› platný index, přitom takto vytvořené indexy
 *      jsou «prázdné»),
 *    ◦ je-li ‹n› typu ‹node_bool› nebo ‹node_int›, skončí
 *      s výjimkou ‹std::domain_error›,
 *  • ‹n.take( idx, t )› totéž jako ‹set›, ale podstrom je z ‹t›
 *    přesunut, tzn. metoda ‹take› nebude žádné uzly kopírovat a po
 *    jejím skončení bude platit ‹t.is_null()›.
 *
 * Všechny metody a návratové hodnoty referenčních typů musí správně
 * pracovat s kvalifikací ‹const›. Vytvoříme-li kopii hodnoty typu
 * ‹tree›, tato bude obsahovat kopii celého stromu. Je-li umožněno
 * kopírování jednotlivých uzlů, nemá určeno konkrétní chování. */
class tree;


class node {
    std::size_t cur;

    public:

    static std::size_t id;
    static std::size_t offset;
    static std::size_t count;

    node() {
        ++count;
        cur = id++;
        printf("node %lu created\n", cur);
    }

    virtual void set(int, const tree &) = 0;
    virtual void take(int, tree &&) = 0;
    virtual void take(int, tree &) = 0;
    virtual node &get(int) = 0;
    virtual const node &get(int) const = 0;
    virtual tree copy() const = 0;
    virtual tree copy(int) const = 0;
    virtual bool is_bool() const = 0;
    virtual bool is_int() const = 0;
    virtual bool is_array() const = 0;
    virtual bool is_object() const = 0;
    virtual int size() const = 0;
    virtual bool as_bool() const = 0;
    virtual int as_int() const = 0;

    void off_print() const {
        for (std::size_t i = 0; i < offset; ++i)
            printf("\t");
    }

    virtual ~node() {
        --count;
        off_print();
        printf("node %lu destroyed, %lu remaining\n", cur, count);
    };
};

std::size_t node::id = 0;
std::size_t node::offset = 0;
std::size_t node::count = 0;

class tree {
    public:
    std::unique_ptr<node> n = nullptr;
    
    bool is_null() const {
        return n == nullptr;
    }

    node &operator*() {
        return *n;
    }

    const node &operator*() const {
        return *n;
    }

    tree() : n{nullptr} {}
    tree(bool);
    tree(int);

    tree(node *na) : n{na} {
        assert(na == n.get());
    }

    tree(tree &&r) {
        std::size_t test = n->id;
        n.swap(r.n);
        r.n.release();
        assert(r.is_null());
        assert(test == n->id);
    }

    tree &operator=(tree &&r) {
        std::size_t test = n->id;
        n.swap(r.n);
        r.n.release();
        assert(r.is_null());
        assert(test == n->id);
        return *this;
    }

    tree &operator=(const tree &r) {
        std::size_t test = n->id;
        n.release();
        *this = r.n->copy();
        assert(n->id > test);
        return *this;
    }
};

class node_bool : public node {
    bool val = false;

    public:
    friend tree;
    node_bool(bool v = false) : val{v} {
        printf("bool created %d\n", v);
    }

    void set(int, const tree &) override {
        throw std::domain_error("bool node is leaf structure");
    };

    void take(int, tree &&) override {
        throw std::domain_error("bool node is leaf structure");
    };

    void take(int, tree &) override {
        throw std::domain_error("bool node is leaf structure");
    };

    node &get(int) override {
        throw std::out_of_range("bool node is leaf structure");
    };

    const node &get(int) const override {
        throw std::out_of_range("bool node is leaf structure");
    };

    tree copy() const override {
        off_print();
        printf("bool %lu is copied\n", id);
        return (val);
    }

    tree copy(int) const override {
        throw std::out_of_range("bool node is leaf structure");
    };

    bool is_bool() const override {
        return true;
    };

    bool is_int() const override {
        return false;
    };

    bool is_array() const override {
        return false;
    };

    bool is_object() const override {
        return false;
    };
    
    int size() const override {
        return 0;
    };

    bool as_bool() const override {
        return val;
    };

    int as_int() const override {
        return static_cast<int>(val);
    };

    ~node_bool() override {
        off_print();
        printf("bool destroyed\n");
    };
};

class node_int : public node {
    int val = 0;

    public:
    friend tree;
    node_int(int v = 0) : val{v} {
        printf("int created %d\n", v);
    }

    void set(int, const tree &) override {
        throw std::domain_error("int node is leaf structure");
    };

    void take(int, tree &&) override {
        throw std::domain_error("int node is leaf structure");
    };

    void take(int, tree &) override {
        throw std::domain_error("int node is leaf structure");
    };

    node &get(int) override {
        throw std::out_of_range("int node is leaf structure");
    };

    const node &get(int) const override {
        throw std::out_of_range("int node is leaf structure");
    };

    tree copy() const override {
        off_print();
        printf("int %lu copied\n", id);
        return (val);
    }

    tree copy(int) const override {
        throw std::out_of_range("int node is leaf structure");
    };

    bool is_bool() const override {
        return false;
    };

    bool is_int() const override {
        return true;
    };

    bool is_array() const override {
        return false;
    };

    bool is_object() const override {
        return false;
    };
    
    int size() const override {
        return 0;
    };

    bool as_bool() const override {
        return static_cast<bool>(val);
    };

    int as_int() const override {
        return val;
    };

    ~node_int() override {
        off_print();
        printf("int destroyed %d\n", val);
    };
};

class node_array : public node {
    std::vector<tree> children;
    int s = 0;

    public:
    friend tree;
    node_array() {
        s = 0;
        children = std::vector<tree>(0);
        printf("array created\n");
    };

    void set(int idx, const tree &t) override {
        if (idx >= size())
            children.resize(idx + 1);
        s += children[idx].is_null();

        children[idx] = (*t).copy();
    };

    void take(int idx, tree &&t) override {
        if (static_cast<std::size_t>(idx) >= children.size())
            children.resize(idx + 1);
        s += children[idx].is_null();
        std::size_t test = id;
        children[idx] = std::move(t);
        assert(test == id);
    };

    void take(int idx, tree &t) override {
        if (static_cast<std::size_t>(idx) >= children.size())
            children.resize(idx + 1);
        s += children[idx].is_null();
        std::size_t test = id;
        children[idx].n.swap(t.n);
        t.n.release();
        assert(t.is_null());
        assert(test == id);
    };

    node &get(int idx) override {
        if (static_cast<std::size_t>(idx) >= children.size())
            throw std::out_of_range("index beyond array scope");
        return *children[idx];
    };

    const node &get(int idx) const override {
        if (static_cast<std::size_t>(idx) >= children.size())
            throw std::out_of_range("index beyond array scope");
        return *children[idx];
    }

    tree copy() const override {
        off_print();
        printf("array %lu copied\n", id);
        ++offset;
        auto *res = new node_array();
        for (const auto &t: children) {
            if (t.is_null()) {
                printf("\n");
                res->children.emplace_back();
            } else {
                res->children.push_back(t.n->copy());
            }
        }
        --offset;
        return res;
    };

    tree copy(int idx) const override {
        if (idx >= size())
            throw std::out_of_range("index beyond array scope");
        printf("%lu ar:", id);
        return children[idx].n->copy();
    };

    bool is_bool() const override {
        return false;
    };

    bool is_int() const override {
        return false;
    };

    bool is_array() const override {
        return true;
    };

    bool is_object() const override {
        return false;
    };

    int size(std::size_t l, std::size_t r) const {
        if (l == r)
            return !children[l].is_null();
        std::size_t mid = (l + r) / 2;
        return size(l, mid) + size(mid + 1, r);
    }

    int size() const override {
        if (children.empty())
            return 0;
        return size(0, children.size() - 1);
    };

    bool as_bool() const override {
        return size() != 0;
    };

    int as_int() const override {
        throw std::domain_error("array isnt convertable to int");
    };

    ~node_array() override {
        off_print();
        printf("array destroyed\n");
        ++offset;
        for (auto &ch: children) {
            if (ch.is_null()) {
                printf("\n");
                continue;
            }
            ch.n.release();
            assert(ch.is_null());
        }
        --offset;
    };
};

class node_object : public node {
    std::map<int, tree> children;

    public:
    friend tree;
    node_object() {
        printf("object created\n");
    };

    void set(int idx, const tree &t) override {
        std::size_t test = id;
        children[idx] = (*t).copy();
        assert(id > test);
    };

    void take(int idx, tree &&t) override {
        std::size_t test = id;
        children[idx] = std::move(t);
        assert(id == test);
    };

    void take(int idx, tree &t) override {
        std::size_t test = id;
        children[idx].n.swap(t.n);
        t.n.release();
        assert(t.is_null());
        assert(test == id);
    };

    node &get(int idx) override {
        if (!children.contains(idx))
            throw std::out_of_range("identifier missing from object");
        return *children[idx];
    };

    const node &get(int idx) const override {
        if (!children.contains(idx))
            throw std::out_of_range("identifier missing from object");
        return *children.at(idx);
    };

    tree copy() const override {
        off_print();
        printf("object %lu copied\n", id);
        ++offset;
        auto *res = new node_object();
        std::size_t test = id;
        for (const auto &[k, t]: children) {
            res->children.emplace(k, t.n->copy());
        }
        --offset;
        assert(id > test);
        return res;
    };

    tree copy(int idx) const override {
        if (!children.contains(idx))
            throw std::out_of_range("identifier missing from object");
        printf("%lu ob: ", id);
        std::size_t test = id;
        auto res = children.at(idx).n->copy();
        assert(id > test); 
        return res;
    };

    bool is_bool() const override {
        return false;
    };

    bool is_int() const override {
        return false;
    };

    bool is_array() const override {
        return false;
    };

    bool is_object() const override {
        return true;
    };

    int size() const override {
        return children.size();
    };

    bool as_bool() const override {
        return !children.empty();
    };

    int as_int() const override {
        throw std::domain_error("object isnt convertable to int");
    };

    ~node_object() override {
        off_print();
        printf("object destroyed\n");
        ++offset;
        for (auto &[_, ch]: children) {
            if (ch.is_null()) {
                printf("\n");
                continue;
            }
            ch.n.release();
            assert(ch.is_null());
        }
        --offset;
    };
};


tree::tree(bool val) : n{new node_bool(val)} {}

tree::tree(int val) : n{new node_int(val)} {}

tree make_bool(bool val = false) {
    return {val};
}

tree make_int(int val = 0) {
    return {val};
}

tree make_array() {
    return {new node_array()};
}

tree make_object() {
    return {new node_object()};
}

int main()
{
    tree tt = make_bool( true ),
         tf = make_bool(),
         ta = make_array(),
         to = make_object();
    printf("\nafter make:\n");
    assert(tt.n->id == 4);

    const tree &c_tt = tt,
               &c_ta = ta,
               &c_to = to;
    printf("\nafter const &:\n");

    auto &na = *ta;
    const auto &c_na = *c_ta;
    auto &no = *to;
    const auto &c_no = no;

    assert(tt.n->id == 4);

    printf("\n1st asserts\n");

    assert( (*c_tt).as_bool() );
    assert( !(*tf).as_bool() );
    assert( !c_na.as_bool() );

    printf("\n2nd asserts\n");

    na.set( 0, ta );
    assert(tt.n->id == 5);
    na.take( 1, make_bool() );
    assert(tt.n->id == 6);


    assert(na.size() == 2);

    assert( !ta.is_null() );
    assert( !c_ta.is_null() );
    assert( !c_to.is_null() );

    assert(tt.n->id == 6);

    printf("\n3nd asserts\n");

    no.set( 1, ta );
    na.take( 1, to );

    assert(na.size() == 2);

    assert( to.is_null() );
    assert( !(*ta).get( 0 ).as_bool() );
    assert( !(*c_ta).get( 0 ).as_bool() );
    assert( c_no.get( 1 ).size() == 2 );

    printf("\ndone\n");

    return 0;
}

// clang-tidy: -clang-analyzer-cplusplus.Move
