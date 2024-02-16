#include <vector>
#include <map>
#include <set>
#include <cassert>

/* V této ukázce se budeme zabývat prohledáváním orientovaného
 * grafu. Asi nejjednodušším vhodným algoritmem je rekurzivní
 * prohledávání do hloubky. Konkrétně nás bude zajímat odpověď na
 * otázku „je vrchol ⟦b⟧ dosažitelný z vrcholu ⟦a⟧?“. Budeme navíc
 * požadovat, aby byla příslušná cesta neprázdná (tzn. ⟦a⟧ budeme
 * považovat za dosažitelné z ⟦a⟧ pouze leží-li na cyklu). */

/* Vstupní graf bude zadaný za pomoci seznamů následníků: typ
 * ‹graph› udává pro každý vrchol grafu jeho následníky. Asociativní
 * kontejner ‹std::map› ukládá dvojice klíč-hodnota a umožňuje mimo
 * jiné efektivně (v logaritmickém čase) nalézt hodnotu podle
 * zadaného klíče.
 *
 * Všimněte si také, že množina vrcholů nemusí nutně sestávat
 * z nepřerušené posloupnosti, nebo jen z malých čisel (proto
 * používáme ‹std::map› a nikoliv ‹std::vector›). */

using edges = std::vector< int >;
using graph = std::map< int, edges >;

/* Krom samotného grafu budeme potřebovat reprezentaci pro «množinu»
 * navštívených vrcholů. V grafu s cykly by algoritmus, který si
 * takovou množinu neudržuje, vedl na nekonečnou rekurzi (nebo
 * nekonečný cyklus). Navíc i v acyklickém grafu bude takový
 * algoritmus vyžadovat (v nejhorším případě) exponenciální čas.
 *
 * Protože sémanticky se jedná o množinu, není asi velkým
 * překvapením, že pro její reprezentaci použijeme asociativní
 * kontejner ‹std::set›. Vyhledání prvku (resp. test na přítomnost
 * prvku) v ‹std::set› má logaritmickou časovou složitost. Podobně
 * tak vložení prvku. */

using visited = std::set< int >;

/* Hlavní rekurzivní podprogram bude potřebovat 2 pomocné parametry:
 * již zmiňovanou množinu navštívených vrcholů, a navíc pravdivostní
 * hodnotu ‹moved›, která řeší případ, kdy potřebujeme zjistit, zda
 * je vrchol dosažitelný sám ze sebe. Naivní řešení by totiž pro
 * dvojici ⟦(a, a)⟧ vždy vrátilo ‹true› (v rozporu s naším zadáním).
 * Proto si v tomto parametru budeme pamatovat, zda jsme se již
 * podél nějaké hrany posunuli.
 *
 * Tento podprogram bude tedy odpovídat na otázku „existuje cesta,
 * která začíná ve vrcholu ‹from›, neprochází žádným vrcholem
 * v ‹seen›, a zároveň končí ve vrcholu ‹to›?“ Všimněte si ale, že
 * množinu ‹seen› předáváme odkazem (referencí) – existuje pouze
 * jediná množina navštívených vrcholů, sdílená všemi rekurzivními
 * aktivacemi podprogramu. Jakmile tedy vrchol potkáme na
 * «libovolné» cestě, bude vyloučen ze zkoumání ve všech ostatních
 * větvích výpočtu (tedy i v těch sourozeneckých, nikoliv jen
 * v potomcích toho současného). */

bool is_reachable_rec( const graph &g, int from, int to,
                       visited &seen, bool moved )
{
    /* První bázový případ je situace, kdy jsme cílový vrchol našli
     * – protože je velmi jednoduchý, vyřešíme jej první. Všimněte
     * si kontroly parametru ‹moved›. */

    if ( from == to && moved )
        return true;

    /* Hlavní cyklus pokrývá zbývající případy:
     *
     *  1. druhý bázový případ, kdy žádný nenavštívený potomek již
     *     neexistuje (tzn. nacházíme se ve slepé větvi a výsledkem
     *     je ‹false›), a
     *  2. případ, kdy existuje dosud nenavštívený soused – pak lze
     *     ale problém vyřešit rekurzí, protože současný vrchol jsme
     *     z problému vyloučili a zbývající problém je tedy menší.
     *
     * Výsledkem volání metody ‹at› je reference na hodnotu
     * přidruženou klíči, který jsme předali v parametru. Proměnná
     * ‹next› tedy nabývá hodnot, které odpovídají přímým
     * následníkům vrcholu ‹from›. */

    for ( auto next : g.at( from ) )

        /* V případě, že jsme nalezli nenavštívený vrchol, nejprve
         * ho označíme za navštívený a poté provedeme rekurzivní
         * volání. Protože jsme se právě posunuli po hraně ‹from,
         * next›, nastavujeme parametr ‹moved› na ‹true›. */

        if ( !seen.contains( next ) )
        {
            seen.insert( next );
            if ( is_reachable_rec( g, next, to, seen, true ) )
                return true;
        }

    /* Skončí-li cyklus jinak, než návratem z podprogramu, znamená
     * to, že jsme vyčerpali všechny možnosti, aniž bychom našli
     * přípustnou cestu, která vrcholy ‹from› a ‹to› spojuje. */

    return false;
}

/* Konečně doplníme jednoduchou funkci, která doplní potřebné
 * hodnoty pomocným parametrům. Odpovídá na otázku „lze se do
 * vrcholu ‹to› dostat podél jedné nebo více hran, začneme-li ve
 * vrcholu ‹to›?“.
 *
 * Za povšimnutí také stojí, že ‹is_reachable› je čistou funkcí (a
 * to i přesto, že ‹is_reachable_rec› čistou funkcí není). */

bool is_reachable( const graph &g, int from, int to )
{
    visited seen;
    return is_reachable_rec( g, from, to, seen, false );
}

int main() /* demo */
{
    graph g{ { 1, { 2, 3, 4 } },
             { 2, { 1, 2 } },
             { 3, { 3, 4 } },
             { 4, {} },
             { 5, { 3 }} };

    assert(  is_reachable( g, 1, 1 ) );
    assert( !is_reachable( g, 4, 4 ) );
    assert(  is_reachable( g, 1, 2 ) );
    assert(  is_reachable( g, 1, 3 ) );
    assert(  is_reachable( g, 1, 4 ) );
    assert( !is_reachable( g, 4, 1 ) );
    assert(  is_reachable( g, 3, 3 ) );
    assert( !is_reachable( g, 3, 1 ) );
    assert(  is_reachable( g, 5, 4 ) );
    assert( !is_reachable( g, 5, 1 ) );
    assert( !is_reachable( g, 5, 2 ) );
}
