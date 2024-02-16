#include <cassert>
#include <deque>
#include <vector>
#include <map>

/* V této ukáce budeme počítat histogram (číselných) ⟦n⟧-gramů, tzn.
 * bloků ⟦n⟧ po sobě jdoucích čísel v nějaké delší sekvenci.
 * Jednotlivé ⟦n⟧-gramy se mohou překrývat (⟦n⟧-gram tedy může
 * začínat na libovolném offsetu).
 *
 * Naším úkolem je navrhnout typ, který bude tuto frekvenci počítat
 * „za běhu“ – počítáme totiž s tím, že vstupních dat bude hodně a
 * budou přicházet po blocích. Zároveň předpokládáme, že různých
 * ⟦n⟧-gramů bude řádově méně než vstupních dat.
 *
 * Budeme implementovat dvě metody:
 *
 *  1. ‹count›, která pro zadaný ⟦n⟧-gram vrátí počet jeho
 *     dosavadních výskytů, a metodu
 *  2. ‹process›, která zpracuje další blok dat. */

struct freq
{
    std::size_t ngram_size = 3;

    /* Budeme potřebovat dvě datové složky: samotné počítadlo
     * výskytů implementujeme pomocí standardního asociativního
     * kontejneru ‹std::map›. Klíčem bude ‹std::vector› potřebné
     * délky (reprezentuje ⟦n⟧-gram), hodnotou pak počet výskytů
     * tohoto ⟦n⟧-gramu. */

    std::map< std::vector< int >, int > _counter;

    /* Druhou složkou bude «posuvné okno», ve kterém budeme
     * uchovávat poslední zpracovaný ⟦n⟧-gram. Je to proto, že
     * některé ⟦n⟧-gramy budou rozděleny mezi dva bloky (nebo i
     * více, pokud se objeví velmi krátký blok). Pro jednoduchost
     * budeme toto okno používat pro všechny ⟦n⟧-gramy, a realizovat
     * ho budeme jako instanci ‹std::deque›¹. */

    std::deque< int > _window;

    /* Nejprve implementujeme pomocnou metodu, která zpracuje jedno
     * číslo. Je-li okno již plné, odstraníme z něj nejstarší
     * hodnotu. Je-li po vložení čísla okno dostatečně plné,
     * výsledný ⟦n⟧-gram započítáme. Vzpomeňte si, že indexovací
     * operátor kontejneru ‹std::map› podle potřeby vloží novou
     * dvojici klíč-hodnota (s hodnotou inicializovanou „na nulu“).
     * */

    void add( int value )
    {
        if ( _window.size() == ngram_size )
            _window.pop_front();

        _window.push_back( value );

        if ( _window.size() == ngram_size )
        {
            std::vector< int > ngram;
            for ( int v : _window )
                ngram.push_back( v );
            ++ _counter[ ngram ];
        }
    }

    /* Protože metoda ‹add› kompletně řeší jak správu okna, tak
     * počítadlo, zpracování bloku už je jednoduché. */

    void process( const std::vector< int > &block )
    {
        for ( int value : block )
            add( value );
    }

    /* Metodu ‹count›, která pouze vrací informace a aktuální objekt
     * nijak nemění, bychom rádi označili jako ‹const›. Jako drobný
     * problém se jeví, že indexace položky ‹_counter› ale není
     * konstantní operace: jak jsme zmiňovali, operátor indexace
     * může do kontejneru vložit novou dvojici, a tím ho změnit.
     *
     * Nemůžeme také přímo použít metodu ‹at›, protože musíme být
     * schopni správně odpovídat i v případě, že dotazovaný ⟦n⟧-gram
     * se na vstupu dosud neobjevil, a tedy takový klíč v kontejneru
     * ‹_counter› není přítomen.
     *
     * Zbývá tedy metoda ‹find›, která nám dá jak informaci o tom,
     * jestli je klíč přítomen (hledání vyžaduje logaritmický čas),
     * a pokud ano, tak nám k němu přímo umožní přístup (již
     * v konstantním čase). Použití s inicializační sekcí
     * podmíněného příkazu ‹if› sze považovat za idiomatické. */

    int count( const std::vector< int > &ngram ) const
    {
        if ( auto it = _counter.find( ngram ); it != _counter.end() )
            return it->second;
        else
            return 0;
    }
};

int main() /* demo */
{
    freq f{ .ngram_size = 3 };

    /* Vytvoříme si na ‹f› také konstantní referenci, abychom se
     * ujistili, že metodu ‹count› skutečně lze volat na konstantní
     * hodnotu. */

    const freq &cf = f;

    assert( cf.count( { 1, 1, 1 } ) == 0 );

    f.process( { 1, 1, 2, 1, 1 } );
    assert( cf.count( { 1, 1, 1 } ) == 0 );
    assert( cf.count( { 1, 1, 2 } ) == 1 );
    assert( cf.count( { 1, 2, 1 } ) == 1 );

    f.process( { 1 } );
    assert( cf.count( { 1, 1, 1 } ) == 1 );
    assert( cf.count( { 1, 2, 1 } ) == 1 );

    f.process( { 1 } );
    f.process( { 2, 2 } );
    assert( cf.count( { 1, 1, 1 } ) == 2 );
    assert( cf.count( { 1, 2, 2 } ) == 1 );
    assert( cf.count( { 2, 2, 1 } ) == 0 );
}

/* ¹ Tato volba reprezentace není úplně nejefektivnější, ale pro
 *   naše účely dostatečná. Asymptoticky jí není co vytknout. */
