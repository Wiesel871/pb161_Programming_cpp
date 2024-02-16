#include <memory>
#include <map>
#include <cassert>

/* Uvažme abstraktní syntaktický strom velmi jednoduchého
 * imperativního programovacího jazyka se strukturovaným řízením
 * toku. Nechť existují 3 typy příkazů:
 *
 *  1. zvýšení proměnné o jedničku, ‹a++›,
 *  2. cyklus while tvaru ‹while (a != b) stmt›, a konečně
 *  3. blok, který je tvořen posloupností příkazů. */

class statement;
using stmt_ptr = std::unique_ptr< statement >;

/* Proměnné budeme označovat písmeny. Rozsah každé proměnné je 0 až
 * 15 včetně; není-li explicitně inicializovaná, její startovní
 * hodnota je 0. Je-li hodnota 15 zvýšena o jedna, výsledkem je opět
 * 0. Metoda ‹eval› dostane na vstupu jednak iniciální nastavení
 * proměnných (hodnotu typu ‹state› definovaného níže), jednak
 * limit ⟦n ≥ 1⟧ na délku výpočtu – tento limit udává kolik může
 * program jako celek vykonat «srovnání». Po provedení ⟦n⟧ srovnání
 * je celý výpočet okamžitě ukončen (tělo cyklu ani žádný jiný
 * příkaz už se neprovede). */

using state = std::map< char, int >;

/* Konstruktory nechť mají tyto parametry:
 *
 *  • ‹stmt_inc› přijme název proměnné, kterou má zvýšit,
 *  • ‹stmt_while› dostane 2 názvy proměnných a tělo ve formě
 *    ukazatele typu ‹stmt_ptr›,
 *  • ‹stmt_block› je implicitně zkonstruovaný jako prázdný, ale
 *    poskytuje metodu ‹append› která na konec bloku přidá příkaz
 *    (opět formou ‹stmt_ptr›). */

class stmt_inc;
class stmt_while;
class stmt_block;

int main()
{
    auto mk_inc = []( auto var )
    {
        return std::make_unique< stmt_inc >( var );
    };

    auto mk_while = []( auto v1, auto v2, auto &&body )
    {
        return std::make_unique< stmt_while >( v1, v2,
                                               std::move( body ) );
    };

    auto body = std::make_unique< stmt_block >();
    body->append( mk_inc( 'a' ) );
    body->append( mk_inc( 'b' ) );

    auto loop = mk_while( 'a', 'c', body );
    std::map< char, int > vars = { { 'a', 0 },
                                   { 'b', 3 },
                                   { 'c', 7 } };

    vars = mk_inc( 'a' )->eval( vars, 1 );
    assert( vars[ 'a' ] == 1 );
    assert( vars[ 'b' ] == 3 );
    assert( vars[ 'c' ] == 7 );

    vars = loop->eval( vars, 5 );
    assert( vars[ 'a' ] == 5 );
    assert( vars[ 'b' ] == 7 );
    assert( vars[ 'c' ] == 7 );

    body = std::make_unique< stmt_block >();
    body->append( std::move( loop ) );
    body->append( mk_inc( 'b' ) );

    loop = mk_while( 'b', 'd', body );
    vars[ 'c' ] = 11;
    vars[ 'd' ] = 15;

    vars = loop->eval( vars, 100 );
    assert( vars[ 'a' ] == 11 );
    assert( vars[ 'b' ] == 15 );
    assert( vars[ 'c' ] == 11 );
    assert( vars[ 'd' ] == 15 );

    return 0;
}
