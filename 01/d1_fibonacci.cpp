#include <cassert>

/* V této ukázce naprogramujeme klasický ukázkový algoritmus, totiž
 * výpočet ⟦n⟧-tého Fibonacciho čísla (a použijeme k tomu iterativní
 * algoritmus). Algoritmus bude implementovat «podprogram» (funkce)
 * ‹fibonacci›. Definice podprogramu se v jazyce C++ začíná tzv.
 * «signaturou» neboli «hlavičkou funkce», která:
 *
 *  1. popisuje návratovou hodnotu (zejména její typ),
 *  2. udává název podprogramu a
 *  3. jeho «formální parametry», opět zejména jejich typy, ale
 *     obvykle i názvy.
 *
 * Signatura může popisovat i další vlastnosti, se kterými se
 * setkáme později.
 *
 * V tomto případě bude návratovou hodnotou «celé číslo»
 * (znaménkového typu ‹int›), podprogram ponese název ‹fibonacci› a
 * má jeden parametr, opět celočíselného typu ‹int›. */

int fibonacci( int n )

    /* Po signatuře následuje tzv. «tělo», které je syntakticky
     * shodné se «složeným příkazem», a je tedy tvořeno libovolným
     * počtem (včetně nuly) příkazů uzavřených do složených závorek.
     * V těle funkce jsou formální parametry (v tomto případě ‹n›)
     * ekvivalentní «lokálním proměnným» pomyslně inicializovaným
     * hodnotou skutečného parametru. */
{
    /* Tělo je tvořeno posloupností příkazů (typický příkaz je
     * ukončen středníkem, ale toto neplatí např. pro složené
     * příkazy, které jsou ukončeny složenou závorkou).
     *
     * Prvním příkazem podprogramu ‹fibonacci› je deklarace
     * lokálních proměnných ‹a›, ‹b›, opět celočíselného typu ‹int›.
     * Deklarace se skládá z:
     *
     *  1. typu, případně klíčového slova ‹auto›,
     *  2. neprázdného seznamu deklarovaných jmen (oddělených
     *     čárkou), které mohou být doplněny tzv. deklarátory
     *     (označují např. reference: uvidíme je v pozdější ukázce),
     *  3. volitelného inicializátoru, který popisuje počáteční
     *     hodnotu proměnné. */

    int a = 1, b = 1, c;

    /* Samotný výpočet zapíšeme pomocí tzv. třídílného ‹for› cyklu
     * (jinou variantu cyklu ‹for› si ukážeme v další kapitole),
     * který má následující strukturu:
     *
     *   1. klíčové slovo ‹for›,
     *   2. hlavička cyklu, uzavřená v kulatých závorkách,
     *      a. inicializační příkaz (výraz, deklarace proměnné, nebo
     *         prázdný příkaz) je vždy ukončen středníkem a provede
     *         se jednou před začátkem cyklu; deklaruje-li proměnné,
     *         tyto jsou platné právě po dobu vykonávání cyklu,
     *      b. podmínka cyklu («výraz» nebo prázdný příkaz) je opět
     *         vždy ukončena středníkem a určuje, zda se má provést
     *         další iterace cyklu (vyhodnotí-li se na ‹true›),
     *      c. «výraz» iterace (výraz, který «není» ukončen
     *         středníkem), který je vyhodnocen «vždy» na konci těla
     *         (před dalším vyhodnocením podmínky cyklu),
     *   3. tělo cyklu (libovolný příkaz, často složený). */

    for ( int i = 2; i < n; ++i )
    {
        /* V jazyce C++ je přiřazení «výraz», kterého vyhodnocení má
         * «vedlejší efekt», a to konkrétně změnu proměnné, která je
         * odkazována levou stranou operátoru ‹=› (jedná se
         * o «výraz», který se musí vyhodnotit na tzv. «l-hodnotu»¹ –
         * «l» od «left», protože stojí na levé straně přiřazení).
         * Na pravé straně pak stojí libovolný «výraz». */

        c = a + b;
        a = b;
        b = c;
    }

    /* Příkaz návratu z podprogramu ‹return› má dvojí význam
     * (podobně jako ve většině imperativních jazyků):
     *
     *  1. určí návratovou hodnotu podprogramu (tato se získá
     *     vyhodnocením «výrazu» uvedeného po klíčovém slově
     *     ‹return›),
     *  2. ukončí vykonávání podprogramu a předá řízení volajícímu.
     */

    return b;
}

/* Všechny ukázky v této sbírce obsahují několik jednoduchých
 * testovacích případů, kterých účelem je jednak předvést, jak lze
 * implementovanou funkcionalitu použít, jednak ověřit, že fungování
 * programu odpovídá naší představě. Zkuste si přiložené testy různě
 * upravovat, abyste si ověřili, že dobře rozumíte tomu, jak ukázka
 * funguje. */

int main() /* demo */
{
    /* Použití (volání) podprogramu je «výraz» a jeho vyhodnocení
     * odpovídá naší intuitivní představě: skutečné parametry
     * (uvedené v kulatých závorkách za jménem) se použijí jako
     * pomyslné inicializátory formálních parametrů a s takto
     * inicializovanými parametry se vykoná tělo podprogramu. Po
     * jeho ukončení se výraz volání podprogramu vyhodnotí na
     * návratovou hodnotu. */

    assert( fibonacci( 1 ) == 1 );
    assert( fibonacci( 2 ) == 1 );
    assert( fibonacci( 7 ) == 13 );
    assert( fibonacci( 20 ) == 6765 );
}

/* ¹ Zjednodušeně, «l-hodnota» je takový výraz, který popisuje
 *   «identitu» resp. «lokaci» – typicky proměnnou, která je uložena
 *   v paměti. L-hodnoty rozlišujeme proto, že smyslem přiřazení je
 *   «uložit» (zapsat) výsledek své pravé strany, a na levé straně
 *   tedy musí stát objekt, do kterého lze tuto pravou stranu
 *   skutečně zapsat. Nejjednodušší l-hodnotou je «název proměnné».
 */
