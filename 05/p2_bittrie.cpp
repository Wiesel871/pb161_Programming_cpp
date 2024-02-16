#include <vector>
#include <cassert>

/* Binární trie je «binární» stom, který kóduje množinu bitových
 * řetězců, s rychlým vkládáním a vyhledáváním. Každá hrana kóduje
 * jeden bit. */

/* Klíč chápeme jako sekvenci bitů – každý bit určuje, kterým směrem
 * budeme ve stromě pokračovat (0 = doleva, 1 = doprava). Bitový
 * řetězec budeme chápat jako přítomný v reprezentované množině
 * právě tehdy, kdy přesně popisuje cestu k listu. Pro jednoduchost
 * budeme klíče reprezentovat jako vektor hodnot typu ‹bool›. */

using key = std::vector< bool >;

struct trie_node;

/* Pro jednoduchost nebudeme programovat klasickou metodu ‹insert›.
 * Místo toho umožníme uživateli přímo vystavět trie pomocí metod
 * ‹root› (zpřístupní kořen trie) a ‹make› (vloží nový uzel:
 * parametry určí rodiče a směr – 0 nebo 1 – ve kterém bude uzel
 * vložen). V obou případech je výsledkem odkaz na uzel, který lze
 * předat metodě ‹make›.
 *
 * Hlavní část úkolu tedy spočívá v implementaci metody ‹has›, která
 * pro daný klíč rozhodne, je-li v množině přítomen. */

struct trie;

int main()
{
    trie x;

    assert( !x.has( key{ true } ) );
    assert( !x.has( key{ false } ) );

    auto &t = x.make( x.root(), true );
    assert(  x.has( key{ true } ) );

    x.make( t, true );
    auto &tf = x.make( t, false );

    assert( !x.has( key{ true } ) );
    assert( !x.has( key{ false } ) );
    assert( !x.has( key{ false, true } ) );
    assert( !x.has( key{ false, false } ) );
    assert(  x.has( key{ true, true } ) );
    assert(  x.has( key{ true, false } ) );
    assert( !x.has( key{ true, false, true } ) );

    x.make( tf, true );
    assert(  x.has( key{ true, false, true } ) );

    return 0;
}
