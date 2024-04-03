#include <cassert>
#include <vector>

/* V tomto cvičení budeme definovat síť počítadel, přičemž každý
 * uzel má jedno počítadlo znaménkového typu, které je iniciálně
 * nastavené na nulu, a události které počítadlo mění se šíří po
 * síti podle pravidel uvedených níže. Každý uzel může mít libovolný
 * počet příchozích i odchozích spojení. */

/* Události jsou tří typů: ‹reset›, který nastaví počítadlo na 0,
 * ‹increment› ho zvýší o jedna a ‹decrement› ho o jedna sníží. */

// gde enum? xd
using event = int;
const event event_reset = 0;
const event event_increment = 1;
const event event_decrement = 2;

/* Abstraktní bázová třída ‹node› určí polymorfní rozhraní:
 *
 *  • ‹react› s jediným argumentem typu ‹event› (popisuje událost
 *    podle konstant výše),
 *  • ‹connect› která přijme odkaz (referenci) na jiný uzel (typ
 *    ‹node›) a vytvoří spojení, které směruje od aktuálního uzlu
 *    k tomu, který je zadaný v parametru,
 *  • ‹read›, «konstantní» metoda, která vrátí aktuální hodnotu
 *    počítadla.
 *
 * Dobře si rozmyslete, které metody muí být virtuální a které
 * nioliv. */

class node {
    int cur = 0;

    protected:
    std::vector<node *> children = {};

    public:
    virtual void react(event e) {
        switch (e) {
            case event_reset:
                cur = 0;
                break;
            case event_decrement:
                --cur;
                break;
            case event_increment:
                ++cur;
                break;
        }
    }

    void connect(node &n) {
        children.emplace_back(&n);
    }

    int read() const {
        return cur;
    }
};

/* Následují již konkrétní typy uzlů. Každý uzel nejprve aplikuje
 * příchozí událost na svoje vlastní počítadlo, poté ho přepošle
 * všem svým sousedům. Implementujte tyto typy:
 *
 *  • ‹forward› přepošle stejnou událost, jakou obrdržel,
 *  • ‹invert› pošle opačnou událost (reset je opačný sám sobě),
 *  • ‹gate› přepošle stejnou událost, ale pouze je-li nová hodnota
 *    počítadla kladná. */

class forward : public node {
    public:
    void react(event e) override {
        node::react(e);
        for (auto &p: children)
            p->react(e);
    }
};

class invert : public node {
    public:
    void react(event e) override {
        node::react(e);
        event inverse = 
            e == event_reset 
            ? event_reset 
            : (e == event_increment
                ? event_decrement
                : event_increment);
        for (auto &p: children)
            p->react(inverse);
    }

};

class gate : public node {
    public:
    void react(event e) override {
        node::react(e);
        if (read() <= 0)
            return;
        for (auto &p: children)
            p->react(e);
    }

};

int main()
{
    const int reset = 0,
              increment = 1,
              decrement = 2;

    forward source, sink_0, sink_1;
    gate g_0, g_1;
    invert inv;

    const invert &c_inv = inv;

    assert( source.read() == 0 );
    assert( c_inv.read() == 0 );

    source.connect( g_0 );
    source.connect( inv );
    inv.connect( g_1 );

    g_0.connect( sink_0 );
    inv.connect( sink_0 );

    source.connect( sink_1 );
    g_0.connect( sink_1 );

    source.react( static_cast<event>( increment ) );

    assert( source.read() == 1 );
    assert( g_0.read()    == 1 );
    assert( inv.read()    == 1 );
    assert( g_1.read()    == -1 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 2 );

    source.react( static_cast<event>( increment ) );

    assert( source.read() == 2 );
    assert( g_0.read()    == 2 );
    assert( inv.read()    == 2 );
    assert( g_1.read()    == -2 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 4 );

    source.react( static_cast<event>( decrement ) );

    assert( source.read() == 1 );
    assert( g_0.read()    == 1 );
    assert( inv.read()    == 1 );
    assert( g_1.read()    == -1 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 2 );

    source.react( static_cast<event>( reset ) );

    assert( source.read() == 0 );
    assert( g_0.read()    == 0 );
    assert( inv.read()    == 0 );
    assert( g_1.read()    == 0 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 0 );

    source.react( static_cast<event>( decrement ) );

    assert( source.read() == -1 );
    assert( g_0.read()    == -1 );
    assert( inv.read()    == -1 );
    assert( g_1.read()    == 1 );
    assert( sink_0.read() == 1 );
    assert( sink_1.read() == -1 );

    return 0;
}
