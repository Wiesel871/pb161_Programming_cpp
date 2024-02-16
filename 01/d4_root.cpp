#include <cassert>

/* † V této poslední ukázce bude naším cílem spočítat celočíselnou
 * ⟦n⟧-tou odmocninu zadaného nezáporného čísla ⟦k⟧. Nejprve ale
 * budeme potřebovat dvě pomocné funkce: celočíselný dvojkový
 * logaritmus a ⟦n⟧-tou mocninu. Vyzbrojeni těmito funkcemi pak
 * budeme schopni odmocninu vypočítat tzv. Newtonovou-Raphsonovou
 * metodou. */

/* Celočíselný dvojkový logaritmus čísla ⟦n⟧ definujeme jako
 * největší celé číslo ⟦k⟧ takové, že ⟦2ᵏ ≤ n⟧. Za povšimnutí stojí,
 * že pro ⟦n < 1⟧ takové ⟦k⟧ neexistuje – proto tuto funkci
 * definujeme pouze pro kladná ⟦n⟧. */

auto int_log2( auto n )
{
    /* Jako obvykle nejprve ověříme vstupní podmínku. */
    assert( n > 0 );

    /* Výpočet budeme provádět v pomocné proměnné, která bude
     * stejného typu jako ‹n›. */

    decltype( n ) result = 0;

    /* Princip výpočtu je jednoduchý, uvážíme-li dvojkový rozvoj
     * čísla ⟦n = ∑aᵢ2ⁱ⟧, který obsahuje člen ⟦2ⁱ⟧ pro každý
     * nenulový bit ⟦aᵢ⟧. Dvojkovým logaritmem bude právě nejvyšší
     * mocnina dvojky, která se v tomto rozvoji objeví: jistě pak
     * platí, že ⟦2ᵏ ≤ n⟧, stačí se ujistit, že takto získané ⟦k⟧ je
     * největší možné. Uvažme tedy, že existuje nějaké ⟦l > k⟧ a
     * zároveň ⟦2ˡ ≤ n⟧. Pak se ale musí ⟦2ˡ⟧ nutně objevit ve
     * dvojkovém rozvoji čísla ⟦n⟧, což je spor s tím, že ⟦k⟧ byla
     * nejvyšší mocnina v témže rozvoji.
     *
     * Stačí nám tedy nalézt řád nejvyššího jedničkového bitu. To
     * provedeme tak, že budeme provádět bitové posuny doprava tak
     * dlouho, až ‹n› vynulujeme. Počet takto provedených posunů je
     * pak hledaný řád. */

    while ( n > 1 )
    {
        ++result;
        n >>= 1;
    }

    return result;
}

/* Jazyk C++ na rozdíl od některých jiných neposkytuje zabudovanou
 * operaci mocnění pro celočíselné typy. Výpočet provedeme známým
 * algoritmem binárního umocňování (anglicky známého popisněji jako
 * „exponentiation by squaring“).¹ Klíčovou vlastností tohoto
 * algoritmu je, že jeho složitost je lineární k «počtu bitů
 * exponentu» – naivní algoritmus opakovaným násobením má naproti
 * tomu složitost «exponenciální» (složitost je v tomto případě
 * přímo úměrná «hodnotě» exponentu, nikoliv délce jeho zápisu). */

auto int_pow( auto n, auto exp )
{
    /* Operaci budeme definovat pouze pro kladné exponenty. Vyhneme
     * se tak mimo jiné nutnosti definovat hodnotu pro ⟦0⁰⟧. */

    assert( exp >= 1 );

    /* Pomocná proměnná, která bude udržovat „liché“ mocniny. Její
     * význam je přesněji vysvětlen níže. */

    decltype( n ) odd = 1;

    /* Výpočet je založený na pozorování, že pro sudý exponent ⟦k⟧
     * platí ⟦nᵏ = n²ˡ = (n²)ˡ⟧ kde ⟦l = k/2⟧. Za cenu výpočtu
     * jedné druhé mocniny – ⟦n²⟧ – tak můžeme exponent snížit na
     * polovinu (cyklus se provede právě tolikrát, kolik bitů je v
     * zápisu hodnoty ‹exp›). */

    while ( exp > 1 )
    {
        /* Musíme ovšem ještě vyřešit situaci, kdy je exponent
         * lichý. Zde je potřebný vztah trochu složitější: ⟦nᵏ =
         * n⋅(n²ˡ)⟧ pro ⟦l = ⌊k/2⌋⟧. V rekurzivním zápisu bychom
         * mohli tento vztah přímo použít, v tom iterativním ale
         * nastane drobný problém: faktor ⟦n⟧ před závorkou
         * «nevstupuje» do výpočtu druhé mocniny v další iteraci.
         * Asi nejjednodušším řešením je použití pomocného střadače,
         * který bude udržovat tyto „přebývající“ faktory. Je-li
         * ‹exp› liché, přinásobíme tedy faktor ‹n› do proměnné
         * ‹odd›. Na konci ovšem nesmíme zapomenout, že ve výsledném
         * ‹n› tyto faktory chybí. */

        if ( exp % 2 == 1 )
            odd *= n;

        /* Dále je výpočet pro sudé i liché exponenty stejný:
         * hodnotu proměnné ‹n› umocníme na druhou a exponent
         * vydělíme dvěma. */

        n *= n;
        exp /= 2;
    }

    /* Na závěr si vzpomeneme, že některé faktory celkového výsledku
     * jsme si „odložili“ do proměnné ‹odd›. */

    return n * odd;

    /* Pro ilustraci uvažme výpočet ⟦3¹⁰⟧:
     *
     *  │ iterace │                     ‹n› │ ‹exp› │ ‹odd› │
     *  ├─────────│─────────────────────────│───────│───────┤
     *  │      0. │                       3 │    10 │     1 │
     *  │      1. │                     3⋅3 │     5 │     1 │
     *  │      2. │             (3⋅3)⋅(3⋅3) │     2 │   3⋅3 │
     *  │      3. │ (3⋅3)⋅(3⋅3)⋅(3⋅3)⋅(3⋅3) │     1 │   3⋅3 │
     *
     * V proměnné ‹n› jsme sesbírali 8 faktorů, zatímco proměnná
     * ‹odd› získala 2, celkem jich je tedy potřebných 10.
     * Rekurzivní výpočet by naproti tomu dopadl takto:
     *
     *    ⟦ (3⋅(3⋅3)⋅(3⋅3)) ⋅ (3⋅(3⋅3)⋅(3⋅3)) ⟧
     *
     * Uvažme ještě výpočet ⟦3¹¹⟧. Je zejména důležité si uvědomit,
     * že faktor, který na daném řádku přidáváme do ‹odd› (je-li
     * ‹exp› na předchozím řádku liché) je právě hodnota ‹n›
     * z tohoto předchozího řádku.
     *
     *  │ iterace │                     ‹n› │ ‹exp› │   ‹odd› │
     *  ├─────────│─────────────────────────│───────│─────────┤
     *  │      0. │                       3 │    11 │       1 │
     *  │      1. │                     3⋅3 │     5 │       3 │
     *  │      2. │             (3⋅3)⋅(3⋅3) │     2 │ 3⋅(3⋅3) │
     *  │      3. │ (3⋅3)⋅(3⋅3)⋅(3⋅3)⋅(3⋅3) │     1 │ 3⋅(3⋅3) │
     *
     * Stejný výpočet rekurzivně:
     *
     *    ⟦ 3 ⋅ (3⋅(3⋅3)⋅(3⋅3)) ⋅ (3⋅(3⋅3)⋅(3⋅3)) ⟧
     */
}

/* Tím se dostáváme k poslední části: samotnému výpočtu celočíselné
 * odmocniny. Budeme ji opět definovat jako největší ⟦s⟧ takové, že
 * ⟦sⁿ ≤ k⟧. */

auto int_nth_root( auto n, auto k )
{
    /* Pro jednoduchost budeme uvažovat pouze odmocniny nezáporných
     * čísel. */

    assert( k >= 0 );
    assert( n >= 1 );

    /* Jednoduché případy vyřešíme zvlášť, protože by nám v obecném
     * výpočtu níže působily určité potíže. */

    if ( n == 1 || k == 0 )
        return k;

    /* Na podrobný popis Newtonovy-Raphsonovy metody (známé též jako
     * metoda tečen) zde nemáme prostor: možná ji znáte z kurzu
     * matematické analýzy, případně si ji můžete vyhledat např. na
     * wikipedii. Pro nás jsou klíčové její základní vlastnosti:
     *
     *  1. metoda nám umožní «rychle» nalézt ⟦x⟧ takové, že pro
     *     zadané ⟦f⟧ platí ⟦f(x) = 0⟧,
     *  2. potřebujeme k tomu samozřejmě definici ⟦f⟧,
     *  3. dále její první derivaci ⟦f'⟧
     *  4. a počáteční odhad hledané hodnoty ⟦x₀⟧.
     *
     * Výpočet opakovaně zlepšuje aktuální odhad ⟦x⟧, a to pomocí
     * vzorce:
     *
     *   ⟦ xᵢ₊₁ = xᵢ - f(xᵢ)/f'(xᵢ) ⟧
     *
     * Vyvstává otázka, jak nám hledání nuly pomůže ve výpočtu
     * odmocniny. K tomu musíme problém přeformulovat. Uvažme
     *
     *   ⟦ f(s) = sⁿ - k ⟧
     *
     * Je-li ⟦f(s) = 0⟧, pak jistě ⟦sⁿ = k⟧, což je ale přesně
     * definice ⟦n⟧-té odmocniny (prozatím té reálné). Potřebujeme
     * ještě derivaci, která je naštěstí velmi jednoduchá:
     *
     *   ⟦ f'(s) = n⋅sⁿ⁻¹ ⟧
     *
     * protože ⟦n⟧ je celočíselná konstanta (pro ⟦n = 1⟧ bychom
     * ovšem narazili na problém). Celkem tedy:
     *
     *   ⟦ sᵢ₊₁ = sᵢ - (sᵢⁿ - k) / (n⋅sᵢⁿ⁻¹) ⟧
     *
     * Nebo výhodněji (přechodem na společný jmenovatel a krácením
     * mocnin ⟦sᵢ⟧):
     *
     *   ⟦ sᵢ₊₁ = tᵢ / n
     *     tᵢ   = (n - 1)⋅sᵢ + k/sⁿ⁻¹ ⟧
     *
     * Zbývá počáteční odhad, který potřebujeme spočítat rychle (a
     * samozřejmě potřebujeme, aby byl co nejblíže výsledné hodnotě
     * ⟦s⟧). Využijeme k tomu dříve definovaný dvojkový logaritmus.
     * Protože ⟦\log(aᵇ) = b⋅\log(a)⟧, můžeme hledanou odmocninu
     * odhadnout jako ⟦2ˡ⁺¹⟧ pro ⟦l = ⌊\log₂(k) / n⌋⟧. Také si
     * všimneme, že tento odhad leží na stejné straně jediného
     * stacionárního bodu funkce ⟦f⟧ (totiž ⟦s = 0⟧) jako skutečné
     * řešení. Nemůže se nám tedy stát, že by výpočet divergoval. */

    decltype( k ) base = 2;
    auto s = base << ( int_log2( k ) / n );

    /* Samotná iterace je po zdlouhavé přípravě už velmi jednoduchá.
     * Zbývají nám k vyřešení dva (související) problémy: kdy
     * iteraci ukončit a jak výpočet provést nad celými čísly.
     * Protože ⟦k > 0⟧, je funkce ⟦f⟧ v kritické oblasti «konvexní»
     * (tečny leží pod grafem). Po první iteraci bude náš odhad tedy
     * celkem jistě příliš velký – průsečík tečny s osou ⟦x⟧ najdeme
     * vpravo od skutečné nuly – a tato situace se už nezmění.
     *
     * V tuto chvíli ale do hry vstupuje skutečnost, že pracujeme
     * s celými a nikoliv reálnými čísly. Výpočet jsme uspořádali
     * tak, aby byl výpočet průsečíku přesný – konkrétně je
     * výsledkem výpočtu dolní celá část jeho reálné hodnoty. Tato
     * dolní celá část sice může být menší, než skutečná hodnota
     * reálné odmocniny, nemůže ale být menší než námi definovaná
     * «celočíselná» odmocnina.
     *
     * Tato pozorování nám konečně umožní formulovat podmínku
     * ukončení: najdeme-li skutečnou celočíselnou odmocninu, další
     * odhad může být buď «stejný nebo větší» než ten předchozí.
     * Tato situace zároveň «nemůže» nastat dříve:
     *
     *  1. z konvexnosti plyne, že odhad, který je příliš velký, se
     *     musí ke skutečnému výsledku provedením iterace přiblížit,
     *  2. protože předchozí vypočtený odhad je vždy celé číslo, musí
     *     sebemenší posun na reálné ose směrem doleva vést ke snížení
     *     jeho «dolní celé části» alespoň o jedničku.
     *
     * Celkově tedy cyklus skončí přesně ve chvíli, kdy začne platit
     * ⟦sⁿ ≤ k⟧. */

    while ( true )
    {
        const auto t = ( n - 1 ) * s + k / int_pow( s, n - 1 );
        const auto s_next = t / n;

        if ( s_next >= s )
            return s;
        else
            s = s_next;
    }
}

int main() /* demo */
{
    assert( int_log2( 1 ) == 0 );
    assert( int_log2( 2 ) == 1 );

    assert( int_pow( 2, 2 ) == 4 );

    assert( int_nth_root( 1, 2 ) == 2 );
    assert( int_nth_root( 2, 4 ) == 2 );
    assert( int_nth_root( 3, 8 ) == 2 );

    for ( int k = 0; k < 1000; ++k )
        for ( int n = 1; n < 20; ++n )
        {
            auto root = int_nth_root( n, k );
            assert( int_pow( root, n ) <= k );
            assert( int_pow( root + 1, n ) > k );
        }

    return 0;
}

/* ¹ Popis algoritmu na české wikipedii je v době psaní tohoto textu
 *   zcela nepoužitelný. Podívejte se raději do té anglické. */
