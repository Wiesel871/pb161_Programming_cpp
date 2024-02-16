#include <vector>
#include <cmath>
#include <cassert>

/* Krom jednoduchých výstupních parametrů (kterými jsme se zabývali
 * v předchozí kapitole) lze uvažovat i o výstupních parametrech
 * složených typů. V této ukázce naprogramujeme funkci ‹primes›,
 * která na konec předaného objektu typu ‹std::vector› vloží všechna
 * prvočísla ze zadaného rozsahu.
 *
 * O parametru ‹out› budeme mluvit jako o výstupním parametru, i
 * když situace je zde o něco složitější: operace, které můžeme
 * se složenou hodnotou provádět, se totiž neomezují pouze na čtení
 * a přiřazení. Musíme tedy vědět, jak závisí chování operací, které
 * chceme provést, na počáteční hodnotě.
 *
 * Například operace vložení prvku na konec vektoru bude fungovat
 * stejně pro libovolný vektor.¹ Protože žádnou jinou operaci
 * s parametrem ‹out› provádět nebudeme, je jeho označení za
 * výstupní parametr opodstatněné. */

void primes( int from, int to, std::vector< int > &out )
{
    for ( int candidate = from; candidate < to; ++ candidate )
    {
        bool prime = true;
        int bound = std::sqrt( candidate ) + 1;

        /* Rozhodování o prvočíselnosti kandidáta provedeme naivně,
         * zkusmým dělením. */

        for ( int div = 2; div < bound; ++ div )
            if ( div != candidate && candidate % div == 0 )
            {
                prime = false;
                break;
            }

        /* Konečně, je-li kandidát skutečně prvočíslem, vložíme ho
         * na konec vektoru odkazovaného parametrem ‹out› (protože
         * ‹out› je referenčního typu, ‹out› je pouze nové jméno pro
         * původní objekt uvedený ve skutečném parametru).
         *
         * Novým prvkem je zde ale zejména «volání metody».
         * Syntakticky se podobá přístupu k položce (viz předchozí
         * ukázka), ale je následováno kulatými závorkami a seznamem
         * parametrů, stejně jako volání běžného podprogramu. Výraz
         * před tečkou se použije jako skrytý parametr metody (ta
         * tedy s výslednou hodnotou může pracovat – zde například
         * volání ‹out.push_back( x )› modifikuje objekt ‹out›).
         * O metodách si toho povíme více v následující kapitole. */

        if ( prime )
            out.push_back( candidate );
    }
}

int main() /* demo */
{
    std::vector< int > p_out;
    std::vector< int > p7 = { 2, 3, 5 },
                       p15 = { 2, 3, 5, 7, 11, 13 };

    primes( 2, 7, p_out );
    assert( p_out == p7 );
    primes( 7, 15, p_out );
    assert( p_out == p15 );
}

/* ¹ Situaci, kdy je vektor „plný“ (obsahuje tolik prvků, že další
 *   nelze přidat, i kdyby to kapacita paměti umožnila) můžeme
 *   zanedbat: na 64b počítači, který skutečně existuje, nemůže
 *   nastat. */
