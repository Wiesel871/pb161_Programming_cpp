#include <cassert>

/* V tomto cvičení se budeme zabývat voláním virtuálních metod
 * zevnitř třídy samotné – přístup, který bychom mohli nazvat
 * „obrácenou“ dědičností. Většina implementace bude totiž
 * v rodičovské třídě, s použitím několika (resp. v tomto příkladu
 * jedné) virtuální metody určené pro vnitřní potřebu. */

/* Naprogramujte jednoduchou dědickou hierarchii tříd, která bude
 * reprezentovat «logický obvod» – součástky spojené vodiči. Každá
 * součástka bude mít nejvýše 2 vstupy a jediný výstup (a všechny
 * budou nabývat jednu ze dvou hodnot – ‹true› nebo ‹false›).
 * Ve třídě ‹component› implementujte tyto «nevirtuální» metody:
 *
 *  • ‹connect› přijme celé číslo (0 nebo 1 – index vstupu, který
 *    hodláme připojovat) a referenci na další součástku, které
 *    výstup připojí na vybraný vstup aktuální součástky,
 *  • ‹read› (bez parametrů), která vrátí aktuální hodnotu výstupu
 *    součástky (tento bude samozřejmě záviset na stavu vstupních
 *    součástek).
 *
 * Implicitně jsou oba vstupy nepřipojené. Nepřipojené vstupy mají
 * pevnou hodnotu ‹false›. Chování není určeno, je-li v obvodu
 * cyklus. */

class component;

/* Dále doplňte tyto odvozené třídy:
 *
 *  • ‹nand› reprezentuje součástku, které výstup je NAND vstupů,
 *  • ‹source› která ignoruje oba vstupy a které výstup je ‹true›,
 *  • ‹delay› která se chová následovně: při prvním volání ‹read›
 *    vrátí vždy ‹false›; další volání ‹read› vrátí hodnotu, kterou
 *    měl vstup 0 při předchozím volání ‹read›. */

class nand;
class source;
class delay;

int main()
{
    nand n;
    delay d_0, d_1;
    source s;

    /* Check the behaviour of components when both their inputs are
     * always ‹false›. */

    assert(  s.read() );
    assert(  n.read() );
    assert( !d_0.read() );
    assert( !d_0.read() );

    /* Let's try a basic connection and the function of a ‹delay›
     * component. */

    d_0.connect( 0, s );
    assert( d_0.read() == false );
    assert( d_0.read() == true );

    n.connect( 0, d_0 );
    n.connect( 1, d_1 );

    assert( n.read() );
    assert( n.read() );

    d_1.connect( 0, s );

    assert(  n.read() );
    assert( !n.read() );

    return 0;
}
