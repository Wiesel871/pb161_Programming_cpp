#include <cassert>

/* Tato ukázka přináší «výstupní parametry» a s nimi «reference».
 * Naším úkolem bude naprogramovat podprogram ‹hamming›, který
 * spočítá tzv. Hammingovu vzdálenost dvou nezáporných čísel ‹a›,
 * ‹b›.  Hammingova vzdálenost se tradičně definuje jako počet
 * znaků, ve kterých se vstupní hodnoty liší.
 *
 * Abychom tedy mohli mluvit o vzdálenosti čísel, musíme je nějak
 * zapsat – v této ukázce k tomu zvolíme dvojkovou soustavu. Protože
 * Hammingova vzdálenost je navíc definovaná pouze pro stejně dlouhá
 * slova, je-li některý dvojkový zápis kratší, doplníme ho pro účely
 * výpočtu levostrannými nulami.
 *
 * Krom samotné vzdálenosti nás bude zajímat také řád nejvyšší
 * číslice, ve které se vstupní čísla liší (rozmyslete si, že taková
 * existuje právě tehdy, je-li výsledná vzdálenost nenulová). Pro
 * tento dodatečný výsledek (který navíc nemusí být vždy definovaný)
 * použijeme již zmiňovaný «výstupní parametr». V případech, kdy
 * definovaný není, nebudeme hodnotu výstupního parametru měnit.
 *
 * Výstupní parametr realizujeme «referencí», kterou zapíšeme za
 * pomoci «deklarátoru» ‹&› – reference na celočíselnou hodnotu typu
 * ‹int› bude tedy ‹int &›.¹ Takto deklarovaná reference zavádí
 * «nové jméno» pro «již existující» objekt. Objeví-li se tedy
 * reference ve «formálním parametru», takto zavedené «jméno» se
 * přímo váže «k hodnotě skutečného parametru».
 *
 * Pro tento skutečný parametr platí stejná omezení, jako pro levou
 * stranu přiřazení (musí tedy být l-hodnotou). Je to proto, že
 * takto zavedený parametr je pouze «novým jménem» pro skutečný
 * parametr. Zejména tedy platí, že kdykoliv se «formální» parametr
 * objeví na «levé straně» přiřazení, toto přiřazení má efekt na
 * «skutečný» parametr. Díky tomu můžeme uvnitř těla změnit hodnotu
 * skutečného parametru. Je-li tedy skutečný parametr např. jméno
 * lokální proměnné ve «volající» funkci, hodnota této proměnné se
 * může provedením «volané» funkce změnit. Rozmyslete si, že
 * u běžných parametrů (které nejsou referencemi) tomu tak není. */

int hamming( auto a, auto b, int &order )
{
    /* Jako obvykle nejprve ověříme vstupní podmínku. */

    assert( a >= 0 && b >= 0 );

    /* Protože pracujeme s dvojkovou reprezentací, můžeme si výpočet
     * zjednodušit použitím vhodných bitových operací. Operátor ‹^›
     * (xor, exclusive or) nastaví na jedničku právě ty bity
     * výsledku, ve kterých se jeho operandy liší. Hledaná
     * Hammingova vzdálenost je tedy právě počet jedniček v binární
     * reprezentaci čísla ‹x›.
     *
     * Všimněte si, že pro lokální proměnnou ‹x› neuvádíme typ –
     * podobně jako v deklaraci parametrů ‹a›, ‹b› jsme zde použili
     * zástupné slovo ‹auto›. Typ takto deklarované proměnné se
     * «odvodí» z jejího inicializátoru (v tomto případě ‹a ^ b›).
     * Na rozdíl od konstrukce ‹decltype› je takto deklarovaná
     * proměnná «vždy hodnotou», i v případě, že pravá strana je
     * referenčního typu.² */

    auto x = a ^ b;

    /* Pro výsledek si zavedeme pomocnou proměnnou ‹result›, do
     * které sečteme počet nenulových bitů. «Pozor!» proměnné
     * jednoduchých typů je nutné inicializovat i v případě, že má
     * být jejich počáteční hodnota nulová. Bez inicializátoru
     * vznikne «neinicializovaná proměnná» kterou je «zakázáno číst»
     * (níže použitý operátor ‹++› „zvětši hodnotu o jedna“
     * samozřejmě svůj operand přečíst musí). */

    int result = 0;

    /* Číslo, které obsahuje alespoň jeden nenulový bit je jistě
     * nenulové – cyklus se tedy bude provádět tak dlouho, dokud
     * jsou v čísle ‹x› nenulové bity. */

    for ( int i = 0; x != 0; ++i, x >>= 1 )

    /* V těle cyklu budeme zkoumat nejnižší bit hodnoty ‹x›, přitom
     * proměnná ‹i› obsahuje jeho «původní» řád. Všimněte si, že
     * ‹for› cyklus je poměrně flexibilní, a že je důležité si jeho
     * hlavičku dobře přečíst: v tomto případě se např. proměnná ‹i›
     * vůbec neobjevuje v podmínce.
     *
     * Naopak výraz iterace má dvě části (oddělené operátorem čárka,
     * který vyhodnotí svůj první operand pouze pro jeho vedlejší
     * efekt – jeho hodnotu zapomene). Efekt na ‹i› je celkem
     * zřejmý, zajímavější je efekt na ‹x›: výraz ‹x >>= 1› provede
     * «bitový posun» proměnné ‹x› o jeden bit doprava. Původní druhý
     * nejnižší bit se tak stane nejnižším, atd., až nejvyšší bit se
     * doplní nulou. Příklad: posunem osmibitové hodnoty ‹10011001›
     * o jednu pozici doprava vznikne hodnota ‹01001100›.
     *
     * Celý cyklus bychom samozřejmě mohli zapsat jako ‹while›
     * cyklus a vyhnuli bychom se tím relativně komplikované
     * hlavičce. Výhodou cyklu ‹for› v tomto případě ale je, že
     * veškeré informace o změnách iteračních proměnných jsou
     * uvedeny na jeho začátku. Čtenář tak už od začátku ví, jaký
     * mají tyto proměnné význam (‹i› se každou iterací zvýší
     * o jedna, ‹x› se bitově posune o jednu pozici doprava) a
     * nemusí tuto informaci zdlouhavě hledat v těle. */

    {
        /* V každé iteraci tak budeme zkoumat «nejnižší bit» hodnoty
         * ‹x› (který odpovídá ‹i›-tému bitu vstupních parametrů
         * ‹a›, ‹b›). S výhodou k tomu použijeme operaci bitové
         * konjunkce (and; na jedničku jsou nastaveny právě ty bity
         * výsledku, které mají hodnotu 1 v obou operandech). Tento
         * operátor zapisujeme znakem ‹&› (pozor, nezaměňujte
         * s deklarátorem reference!). */

        if ( x & 1 )
        {
            /* Je-li nejnižší bit nastavený, zvýšíme hodnotu proměnné
             * ‹result› a do výstupního parametru ‹order› zapíšeme jeho
             * původní řád (který si udržujeme v proměnné ‹i›).
             *
             * Protože bity procházíme v pořadí od nejnižšího
             * k nejvyššímu, poslední zápis do parametru ‹order› přesně
             * odpovídá nejvyššímu rozdílnému bitu. Podmínka cyklu nám
             * navíc zaručuje, že do proměnné ‹order› zapíšeme pouze
             * v případě, že takový bit existuje. */

            ++result;
            order = i;
        }
    }

    /* Po ukončení cyklu platí, že jsme zpracovali všechny nenulové
     * bity ‹x›, a tedy všechny bity, ve kterých se hodnoty ‹a›, ‹b›
     * lišily. Nezbývá, než nastavit návratovou hodnotu a podprogram
     * ukončit. */

    return result;

    /* Všimneme si ještě, že hodnotu parametru ‹order› jsme
     * «nečetli». «Definující» vlastností výstupních parametrů je,
     * že chování podprogramu «nezávisí» na jejich počáteční
     * hodnotě. V případě, že jediná operace, kterou s výstupním
     * parametrem provedeme, je přiřazení do něj, je tento požadavek
     * triviálně splněn. */
}

int main() /* demo */
{
    int order = 3;

    /* Protože skutečný parametr ‹order› předáváme referencí
     * (odpovídající formální parametr je referenčního typu), změny,
     * které v něm podprogram ‹hamming› provede, jsou viditelné i
     * navenek. Nejprve ovšem ověříme, že při nulové vzdálenosti se
     * hodnota ‹order› nemění. */

    assert( hamming( 0, 0, order ) == 0 && order == 3 );
    assert( hamming( 1, 1, order ) == 0 && order == 3 );

    /* Hodnoty v dalším příkladě se liší ve dvou bitech (osmém a
     * devátém) a proto očekáváme, že po provedení funkce ‹hamming›
     * bude mít proměnné ‹order› hodnotu 9. */

    assert( hamming( 512, 256, order ) == 2 && order == 9 );
    assert( hamming( 0, 1, order ) == 1 && order == 0 );

    assert( hamming( 0xffffff, 0, order ) == 24 );
    assert( hamming( 0xffffffffff, 0, order ) == 40 );
    assert( hamming( 0xf000000000, 0xf, order ) == 8 );
    assert( hamming( 0xf000000000, 0xe000000000, order ) == 1 );

    return 0;
}

/* ¹ To, že se jedná o referenci, je součástí typu takto zavedené
 *   proměnné (resp. parametru) – projeví se to např. při použití
 *   konstrukce ‹decltype›. Zároveň ale platí, že ve většině případů
 *   jsou reference a hodnoty záměnné: je to mimo jiné proto, že na
 *   výsledek «libovolného výrazu» lze nahlížet jako na určitý druh
 *   reference. Blíže se budeme referencemi zabývat ve čtvrté
 *   kapitole.
 * ² To opět souvisí s tím, že každý výraz lze interpretovat jako
 *   referenci. Chování tohoto typu deklarace je uzpůsobeno tomu, že
 *   obvykle chceme deklarovat lokální proměnné – lokální reference
 *   jsou mnohem vzácnější. */
