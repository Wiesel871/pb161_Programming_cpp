#include <cassert> /* assert */
#include <cstdint> /* int64_t */
#include <algorithm> /* min, max */

/* V této ukázce se zaměříme na vlastnosti celočíselných typů.
 * Podíváme se přitom na «kombinační čísla», definovaná jako:
 *
 *  ⟦ (n¦k) = n! / (k! ⋅ (n - k)!) ⟧
 *
 * kde ⟦k ≤ n⟧. Samozřejmě, mohli bychom počítat kombinační čísla
 * přímo z definice, má to ale jeden důležitý problém: celočíselné
 * proměnné mají v C++ pevný rozsah. Výpočet mezivýsledku ⟦n!⟧ tak
 * může velmi lehce překročit horní hranici použitého typu, a to i
 * v případech, kdy celkový výsledek není problém reprezentovat.
 *
 * Proto je důležité najít formu výpočtu, která nebude vytvářet
 * zbytečně velké mezivýsledky. Výpočet kombinačního čísla lze navíc
 * provádět na libovolném celočíselném typu (včetně těch
 * bezznaménkových), proto čistou funkci ‹comb› definujeme tak, aby
 * fungovala pro všechny takové typy.
 *
 * Parametr uvedený klíčovým slovem ‹auto› může být libovolného typu
 * (použití funkce s takovým typem parametru, pro který «tělo»
 * funkce není typově správné, překladač zamítne). Něco jiného
 * znamená «návratová hodnota» deklarovaná jako ‹auto›: tento typ se
 * odvodí z příkazů ‹return› v těle funkce. Chceme-li toto tzv.
 * «odvození návratového typu» využít, musí mít výraz u všech
 * příkazů ‹return› v těle stejný typ. */

auto comb( auto n, auto k )
{
    /* Kombinační čísla jsou definovaná pouze pro ⟦n ≥ k⟧ a tuto
     * vstupní podmínku si můžeme lehce ověřit «tvrzením»: */

    assert( n >= k );

    /* Výpočet budeme provádět na stejném typu, jaký má vstupní ‹n›.
     * Protože tento typ neznáme, musíme si pomoct konstrukcí
     * ‹decltype›, která nám umožní vytvořit proměnnou stejného
     * typu, jako nějaká existující.
     *
     * «Pozor!» Je-li původní proměnná referencí, bude i nová
     * proměnná referencí.
     * «Pozor!» Nemůžeme zde použít ‹auto result = 1›. Proč? */

    decltype( n ) result = 1;

    /* Jak jistě víte, faktoriál je definován takto:
     *
     *  ⟦ n! = ∏ᵢ₌₁ⁿi ⟧
     *
     * A tedy:
     *
     *  ⟦ n! / k! = ∏ᵢ₌₁ⁿ i / ∏ᵢ₌₁ᵏ i = ∏ᵢ₌ₖ₊₁ⁿ i ⟧
     *
     * Tento výpočet bychom jednoduše zapsali do ‹for› cyklu
     * v příslušných mezích. Ve skutečnosti ale můžeme výpočet ještě
     * znatelně zlepšit.
     *
     * Klíčové pozorování je, že ani zbývající ⟦(n - k)!⟧ není
     * potřeba vyčíslovat. Víme jistě, že výsledek bude celé číslo,
     * tzn. všechny faktory ⟦(n - k)!⟧ se musí pokrátit s nějakými
     * faktory ⟦n!/k!⟧. Jedna možnost je seřadit faktory čitatele
     * sestupně a faktory jmenovatele vzestupně a mezivýsledek
     * střídavě násobit a dělit příslušným faktorem (celočíselnost
     * mezivýsledků je zde zaručena tím, že jsou to opět kombinační
     * čísla, jak lze nahlédnout např. rozšířením příslušných zlomků
     * vhodným faktoriálem).
     *
     * Toto řešení je optimální v počtu aritmetických operací, není
     * ale optimální ve velikosti mezivýsledku. Přesněji, je-li
     * ⟦(n¦h)⟧ největší kombinační číslo s daným ⟦n⟧, největší
     * mezivýsledek při výpočtu ⟦(n¦k)⟧ bude ⟦h⋅(n¦h)⟧. Využijeme-li
     * navíc symetrie ⟦(n¦k) = (n¦n - k)⟧, můžeme tuto mez zlepšit na
     * ⟦k⋅(n¦k)⟧ a zároveň zabezpečit, že ⟦k ≤ h⟧. Je nicméně
     * zřejmé, že výpočet nám může přetéct i v případě, kdy celkový
     * výsledek reprezentovat lze. */

    /* Proměnné ‹nom_f› a ‹denom_f› budou reprezentovat aktuální
     * faktory v čitateli a jmenovateli. Opět budou stejného typu
     * jako vstupní ‹n›. */

    decltype( n ) nom_f = n, denom_f = 1;

    /* Cyklus provedeme pro ‹nom_f› v klesajícím rozsahu ⟦⟨n, k)⟧
     * resp. ⟦⟨n, n - k)⟧, podle toho která spodní mez je větší.
     * Protože jednotlivé mezihodnoty na spodní hranici iterace
     * nezávisí, je jistě výhodnější provést méně iterací. */

    while ( nom_f > std::max( k, n - k ) )
    {
        /* Máme-li cyklus zapsaný správně, faktor jmenovatele nemůže
         * překročit «menší» z hodnot ⟦k⟧ nebo ⟦n - k⟧. O tomto se
         * opět ujistíme tvrzením. */

        assert( denom_f <= std::min( k, n - k ) );

        /* Dále provedeme samotný krok výpočtu. Tvrzením se
         * ujistíme, že provádíme skutečně pouze celočíselná dělení
         * beze zbytku (kdyby tomu tak nebylo, výpočet by byl
         * nesprávný!). */

        result *= nom_f;
        assert( result % denom_f == 0 );
        result /= denom_f;

        /* Nakonec upravíme iterační proměnné a pokračujeme další
         * iterací. */

        --nom_f;
        ++denom_f;
    }

    return result;
}

int main() /* demo */
{
    assert( comb( 1, 1 ) == 1 );
    assert( comb( 2, 1 ) == 2 );
    assert( comb( 5, 2 ) == 10 );

    /* Postup implementovaný podprogramem ‹comb› nám umožňuje
     * spočítat, za pomoci 64-bitových proměnných, všechna
     * kombinační čísla pro ⟦n ≤ 60⟧, a to i přesto, že nejen ⟦60! ≈
     * 1,1⋅2²⁷²⟧, ale ⟦60!/30! ≈ 1,34⋅2¹⁶⁴⟧ a tedy ani toto mnohem
     * menší číslo se do 64-bitové proměnné v žádném případě
     * nevejde.
     *
     * Poznámka: typ ‹std::int64_t› je právě 64-bitový celočíselný
     * typ se znaménkem. Abychom ho zde mohli použít, museli jsme
     * výše vložit hlavičku ‹cstdint›. */

    for ( std::int64_t i = 1; i < 60; ++i )
        for ( std::int64_t k = 1; k < i; ++k )
            assert( comb( i + 1, k + 1 ) ==
                    comb( i, k ) + comb( i, k + 1 ) );

    return 0;
}
