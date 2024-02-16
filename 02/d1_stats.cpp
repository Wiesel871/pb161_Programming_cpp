#include <vector>
#include <cmath>
#include <cassert>

/* V této ukázce spočítáme několik jednoduchých statistických
 * veličin – míry polohy (průměr, medián) a variance (směrodatnou
 * odchylku). Využijeme k tomu záznamové typy a sekvenční typ
 * ‹std::vector›. Nejprve si definujeme typ pro výsledek naší
 * jednoduché analýzy – použijeme k tomu záznamový typ, který
 * deklarujeme klíčovým slovem ‹struct›, názvem, a seznamem
 * deklarací složek uzavřeným do složených závorek (a jako všechny
 * deklarace, ukončíme i tuto středníkem): */

struct stats
{
    double median = 0.0;
    double mean = 0.0;
    double stddev = 0.0;
};

/* Tím máme definovaný nový typ s názvem ‹stats›, který můžeme dále
 * používat jako libovolný jiný typ (zabudovaný, nebo ze standardní
 * knihovny). Zejména můžeme vytvářet hodnoty tohoto typu, předávat
 * je jako parametry nebo vracet jako výsledky podprogramů.
 *
 * V této ukázce si zadefinujeme čistou funkci ‹compute_stats›,
 * která potřebné veličiny spočítá a vrátí je jako hodnotu typu
 * ‹stats›. Vstupní parametr ‹data› předáme «konstantní» referencí:
 * hodnoty nebudeme nijak měnit (programujeme čistou funkci a ‹data›
 * považujeme za vstupní parametr). Zároveň nepotřebujeme vytvořit
 * kopii vstupních dat – budeme je pouze číst, taková kopie by tedy
 * byla celkem zbytečná a potenciálně drahá (dat, které chceme
 * zpracovat, by mohlo být mnoho). */

stats compute_stats( const std::vector< double > &data )
{
    int n = data.size();
    double sum = 0, square_error_sum = 0;
    stats result;

    /* Na tomto místě se nám bude hodit nový prvek řízení toku,
     * kterému budeme říkat stručný ‹for› cyklus (angl. „range
     * ‹for›“). Jeho účelem je procházet posloupnost hodnot uloženou
     * v «iterovatelném typu» (použitelnost hodnoty ve stručném
     * ‹for› cyklu lze chápat přímo jako definici iterovatelného
     * typu). Do závorky uvádíme deklaraci proměnné cyklu (můžeme
     * zde použít zástupné slovo ‹auto›) a dvojtečkou oddělený
     * «výraz». Tento výraz musí být iterovatelného typu a výsledná
     * iterovatelná hodnota je ta, kterou budeme cyklem procházet. */

    for ( double x_i : data )

        /* Cyklus se provede pro každý prvek předané iterovatelné
         * hodnoty. Tento prvek je pokaždé uložen do proměnné cyklu
         * (která může být referencí – v takovém případě tato
         * reference odkazuje přímo na prvek, v opačném případě se
         * jedná o kopii). */

        sum += x_i;

    /* K položkám hodnoty záznamového typu přistupujeme «výrazem»
     * ‹expr.field›, kde:
     *
     *  • ‹expr› je «výraz» záznamového typu (zejména to tedy může
     *    být název proměnné), následovaný
     *  • tečkou (technicky se jedná o operátor s vysokou prioritou),
     *  • ‹field› je «jméno» atributu (tzn. na pravé straně tečky
     *    «nestojí výraz»).
     *
     * Je-li výraz ‹expr› l-hodnotou, je l-hodnotou i výraz přístupu
     * k položce jako celek a lze do něj tedy přiřadit hodnotu. */

    result.mean = sum / n;

    /* Medián získáme dobře známým postupem. Za povšimnutí stojí
     * «indexace» vektoru ‹data› zápisem indexu do hranatých
     * závorek. Obecněji jsou-li ‹x› a ‹i› výrazy, je výraz také ‹x[
     * i ]› kde ‹x› je indexovatelného typu (omezení na typ ‹i›
     * závisí na typu ‹x›). Je-li ‹x› navíc l-hodnota, je l-hodnotou
     * i výraz ‹x[ i ]› jako celek.¹ * */

    if ( n % 2 == 1 )
        result.median = data[ n / 2 ];
    else
        result.median = ( data[ n / 2 ] + data[ n / 2 - 1 ] ) / 2;

    /* Konečně spočítáme směrodatnou odchylku. K tomu budeme
     * potřebovat dříve vypočítaný průměr. */

    for ( double x_i : data )
        square_error_sum += std::pow( x_i - result.mean, 2 );

    double variance = square_error_sum / ( n - 1 );
    result.stddev = std::sqrt( variance );

    return result;
}

int main() /* demo */
{
    std::vector< double > sample = { 2, 4, 4, 4, 5, 5, 5, 7, 9 };
    auto s = compute_stats( sample );

    assert( s.mean == 5 );
    assert( s.median == 5 );
    assert( s.stddev == 2 );

    sample.push_back( 1100 );
    s = compute_stats( sample );

    assert( s.median == 5 );
    assert( s.mean > 100 );
    assert( s.stddev > 100 );
}

/* ¹ V některých případech je ‹x[ i ]› l-hodnotou i v případě, že
 *   ‹x› samotné l-hodnota není (opačná implikace tedy obecně
 *   neplatí). Výslednou l-hodnotu ale stejně nelze smysluplně
 *   použít. */
