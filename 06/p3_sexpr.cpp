#include <memory>
#include <cassert>
#include <vector>

/* An s-expression is a tree in which each node has an arbitrary
 * number of children. To make things a little more interesting, our
 * s-expression nodes will own their children. */

/* The base class will be called ‹node› (again) and it will have
 * single (virtual) method: ‹value›, with no arguments and an ‹int›
 * return value. */

class node {
    public:
    virtual int value() const {
        return 0;
    }
};

using node_ptr = std::unique_ptr< node >;

/* There will be two types of internal nodes: ‹sum› and ‹product›,
 * and in this case, they will compute the sum or the product of all
 * their children, regardless of their number. A sum with no
 * children should evaluate to 0 and a product with no children
 * should evaluate to 1.
 *
 * Both will have an additional method: ‹add_child›, which accepts
 * (by value) a single ‹node_ptr› and both should have default
 * constructors. It is okay to add an intermediate class to the
 * hierarchy. */

class binary : public node {
    protected:
    std::vector<node_ptr> children = {};

    public:
    void add_child(node_ptr p) {
        children.push_back(std::move(p));
    }
};

class sum : public binary {
    public:
    int value() const override {
        int res = 0;
        for (auto &p: children)
            res += p->value();
        return res;
    }
};

class product : public binary {
    public:
    int value() const override {
        int res = 1;
        for (auto &p: children)
            res *= p->value();
        return res;
    }
};

/* Leaf nodes carry an integer constant, given to them via a
 * constructor. */

class constant : public node {
    int x;
    
    public:
    constant(int x) : x{x} {}

    int value() const override {
        return x;
    }
};

int main()
{
    auto mk_const = []( int n )
    {
        return std::make_unique< constant >( n );
    };

    auto mk_sum = []{ return std::make_unique< sum >(); };
    auto mk_prod = []{ return std::make_unique< product >(); };

    auto s = mk_sum();
    auto p = mk_prod();

    s->add_child( mk_const( 2 ) );
    s->add_child( mk_const( 3 ) );

    assert( s->value() == 5 );

    p->add_child( mk_const( 2 ) );
    p->add_child( std::move( s ) );

    assert( p->value() == 10 );

    p->add_child( mk_const( 7 ) );

    assert( p->value() == 70 );

    s = mk_sum();
    assert( s->value() == 0 );

    s->add_child( std::move( p ) );
    assert( s->value() == 70 );

    s->add_child( mk_const( 30 ) );
    assert( s->value() == 100 );

    return 0;
}
