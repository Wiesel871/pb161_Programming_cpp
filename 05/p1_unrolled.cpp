#include <array>
#include <cassert>
#include <cstdint>
#include <memory>

/* Předmětem tohoto cvičení je datová struktura, tzv. „rozbalený“
 * zřetězený seznam. Typ, který bude strukturu zastřešovat, by měl
 * mít metody ‹begin›, ‹end›, ‹empty› a ‹push_back›. Ukládat budeme
 * celá čísla.
 *
 * Rozdíl mezi běžným zřetězeným seznamem a rozbaleným seznamem
 * spočívá v tom, že ten rozbalený udržuje v každém uzlu několik
 * hodnot (pro účely tohoto příkladu 4). Samozřejmě, poslední uzel
 * nemusí být zcela zaplněný. Aby měla taková struktura smysl,
 * požadujeme, aby byly hodnoty uloženy přímo v samotném uzlu, bez
 * potřeby další alokace paměti.
 *
 * Návratová hodnota metod ‹begin› a ‹end› bude „pseudo-iterátor“:
 * bude poskytovat prefixový operátor zvětšení o jedničku
 * (pre-increment), rovnost a operátor dereference. Více informací
 * o tomto typu objektu naleznete například v ukázce ‹d1_queue›.
 *
 * V tomto příkladu není potřeba implementovat mazání prvků. */

struct unrolled_node {
    unrolled_node *next = nullptr;
    std::uint8_t len = 0;
    std::array<int, 4> ar = {};
};

struct unrolled_iterator {
    unrolled_node *cur = nullptr;
    std::uint8_t i = 0;

    int operator++() {
        if (i == cur->len - 1) {
            cur = cur->next;
            i = 0;
        } else {
            ++i;
        }
        if (!cur)
            return 0;
        return cur->ar[i];
    }

    int *operator->() {
        return &(cur->ar[i]);
    }

    int &operator*() {
        return cur->ar[i];
    }

    bool operator!=(const unrolled_iterator &r) {
        return cur != r.cur || i != r.i;
    }
};

struct unrolled {
    unrolled_node *first = nullptr, *last = nullptr;

    bool empty() const {
        return first == nullptr;
    }

    unrolled_iterator begin() const {
        return {first};
    }

    unrolled_iterator end() const {
        return {nullptr};
    }

    void push_back(int x) {
        if (!last) {
            first = new unrolled_node;
            last = first;
        } else if (last->len == 4) {
            last->next = new unrolled_node{};
            last = last->next;
        }
        
        last->ar[last->len] = x;
        last->len += 1;
    }

    ~unrolled() {
        unrolled_node *next = nullptr;
        for (unrolled_node *p = first; p; p = next) {
            next = p->next;
            delete p;
        }
    }

};

int main()
{
    /* Basic properties of an empty list. */
    unrolled u;
    assert( u.empty() );
    assert( !( u.begin() != u.end() ) );

    /* Basic properties of a non-empty list. */

    u.push_back( 3 );
    assert( !u.empty() );
    auto i = u.begin();

    assert( i != u.end() );
    ++ i;
    assert( !( i != u.end() ) );

    /* Helper function to check the content of the list. */

    auto check = [&]( int total )
    {
        int count = 0;
        for ( int x : u )
            assert( x == count++ + 3 );
        assert( count == total );
    };

    /* Add things to the list and check they are there. */

    check( 1 );

    for ( int i = 0; i < 10; ++i )
    {
        u.push_back( 4 + i );
        check( 2 + i );
    }

    return 0;
}
