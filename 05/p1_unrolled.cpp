#include <cassert>

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

struct unrolled_node;
struct unrolled_iterator;
struct unrolled;

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
