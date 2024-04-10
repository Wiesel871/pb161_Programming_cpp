#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
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
    public:

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
    virtual void print(int t = 0) const = 0;

    void print_off(int t) const {
        for (int i = 0; i < t; ++i)
            printf("\t");
    }

    virtual ~node() = default;
};

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
        n.swap(r.n);
        r.n.reset();
        assert(r.is_null());
    }

    tree(const tree &r) {
        if (r.is_null()) {
            n = nullptr;
            return;
        }
        *this = r.n->copy();
    }

    tree &operator=(tree &&r) {
        n.swap(r.n);
        r.n.reset();
        assert(r.n == nullptr);
        assert(r.is_null());
        return *this;
    }

    tree &operator=(const tree &r) {
        assert(n == nullptr);
        tree c = r.n->copy();
        *this = std::move(c);
        assert(r.n);
        assert(!r.is_null());
        return *this;
    }
};

class node_bool : public node {
    bool val = false;

    public:
    friend tree;
    node_bool(bool v = false) : val{v} {}

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

    void print(int t = 0) const override {
        print_off(t);
        printf("bool %d\n", val);
    }

    ~node_bool() override = default;
};

class node_int : public node {
    int val = 0;

    public:
    friend tree;
    node_int(int v = 0) : val{v} {}

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

    void print(int t = 0) const override {
        print_off(t);
        printf("int %d\n", val);
    }

    ~node_int() override = default;
};

class node_array : public node {
    std::vector<std::unique_ptr<node>> children;

    public:
    friend tree;
    node_array() {
        children = std::vector<std::unique_ptr<node>>(0);
    };

    void set(int idx, const tree &t) override {
        if (static_cast<std::size_t>(idx) >= children.size())
            children.resize(idx + 1);
        if (t.is_null()) {
            children[idx].reset();
            return;
        }
        tree c = (*t).copy();
        children[idx].swap(c.n);
        c.n.reset();
        assert(!t.is_null());
    };

    void take(int idx, tree &&t) override {
        if (static_cast<std::size_t>(idx) >= children.size())
            children.resize(idx + 1);
        if (t.is_null()) {
            children[idx].reset();
            return;
        }
        children[idx].swap(t.n);
        t.n.reset();
        assert(t.is_null());
    };

    void take(int idx, tree &t) override {
        if (static_cast<std::size_t>(idx) >= children.size())
            children.resize(idx + 1);
        if (t.is_null()) {
            children[idx].reset();
            return;
        }
        children[idx].swap(t.n);
        t.n.reset();
        assert(t.is_null());
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
        auto *res = new node_array();
        for (const auto &p: children) {
            if (!p) {
                res->children.emplace_back();
            } else {
                tree c = p->copy();
                res->children.emplace_back();
                res->children.back().swap(c.n);
                c.n.reset();
            }
        }
        return res;
    };

    tree copy(int idx) const override {
        if (static_cast<std::size_t>(idx) >= children.size())
            throw std::out_of_range("index beyond array scope");
        return children[idx]->copy();
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
            return children[l] != nullptr;
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

    void print(int t = 0) const override {
        print_off(t);
        printf("array\n");
        for (std::size_t i = 0; i < children.size(); ++i) {
            print_off(t + 1);
            printf("%lu\n", i);
            if (!children[i]) {
                printf("\n");
                continue;
            }
            children[i]->print(t + 1);
        }
    }

    ~node_array() override = default;
};

class node_object : public node {
    std::map<int, std::unique_ptr<node>> children;

    public:
    friend tree;
    node_object() = default;

    void set(int idx, const tree &t) override {
        if (t.is_null()) {
            children[idx].reset();
            return;
        }
        tree c = (*t).copy();
        children[idx].swap(c.n);
        c.n.reset();
        assert(!t.is_null());
    };

    void take(int idx, tree &&t) override {
        children[idx].swap(t.n);
        t.n.reset();
    };

    void take(int idx, tree &t) override {
        children[idx].swap(t.n);
        t.n.reset();
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
        auto *res = new node_object();
        for (const auto &[k, t]: children) {
            res->children.emplace(k, t->copy().n);
        }
        return res;
    };

    tree copy(int idx) const override {
        if (!children.contains(idx))
            throw std::out_of_range("identifier missing from object");
        return children.at(idx)->copy();
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
        int res = 0;
        for (const auto &[_, t]: children)
            res += t != nullptr;
        return res;
    };

    bool as_bool() const override {
        return !children.empty();
    };

    int as_int() const override {
        throw std::domain_error("object isnt convertable to int");
    };

    void print(int t = 0) const override {
        print_off(t);
        printf("object\n");
        for (const auto &[k, ch]: children) {
            print_off(t + 1);
            printf("%d\n", k);
            if (!ch.get()) {
                printf("\n");
                continue;
            }
            ch->print(t + 1);
        }
    }

    ~node_object() override = default;
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

static std::size_t tid = 0;

std::tuple<std::size_t, tree> object_id() {
    tree t{};
    return std::tuple<std::size_t, tree>{tid++, t};
}

int main()
{
    tree tt = make_bool( true ),
         tf = make_bool(),
         ta = make_array(),
         to = make_object();
    printf("\nafter make:\n");

    const tree &c_tt = tt,
               &c_ta = ta,
               &c_to = to;
    printf("\nafter const &:\n");

    auto &na = *ta;
    const auto &c_na = *c_ta;
    auto &no = *to;
    const auto &c_no = no;


    printf("\n1st asserts\n");

    assert( (*c_tt).as_bool() );
    assert( !(*tf).as_bool() );
    assert( !c_na.as_bool() );

    printf("\n2nd asserts\n");

    na.set( 0, ta );
    na.take( 1, make_bool() );


    assert(na.size() == 2);

    assert( !ta.is_null() );
    assert( !c_ta.is_null() );
    assert( !c_to.is_null() );


    printf("\n3nd asserts\n");

    no.set( 1, ta );
    na.take( 1, to );

    assert(na.size() == 2);

    assert( to.is_null() );
    assert( !(*ta).get( 0 ).as_bool() );
    assert( !(*c_ta).get( 0 ).as_bool() );
    assert( c_no.get( 1 ).size() == 2 );

    printf("\ndone\n");

    auto &root = na;

    root.get(1).take(5, make_int());
    //root.print();
    assert(root.get(1).size() == 2);
    assert(root.get(1).get(1).is_array());
    root.take(3, make_array());
    //root.print();
    tree t{};
    t = make_bool( true );
    auto &n = *t;
    assert(!t.is_null());

    assert(n.is_bool());
    assert(!n.is_int());
    assert(!n.is_array());
    assert(!n.is_object());
    assert(n.as_int() == 1);
    assert(n.as_bool() == true);
    assert(n.size() == 0);
    assert(!t.is_null());
    tree t2 { t };
    assert(n.is_bool());
    assert(!n.is_int());
    assert(!n.is_array());
    assert(!n.is_object());
    assert(n.as_int() == true);
    assert(n.as_bool() == true);
    assert(n.size() == 0);
    assert(!t.is_null());

    tree rc(ta);
    assert(!ta.is_null());

    t = make_object();
    (*t).set(0, ta);
    assert(&root == ta.n.get());
    (*t).take(1, ta);
    assert(ta.is_null());
    (*t).print();

    auto [id, tr] = object_id();

    return 0;
}

// clang-tidy: -clang-analyzer-cplusplus.Move
