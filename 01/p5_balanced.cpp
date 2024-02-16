#include <cassert>

/* V této úloze budeme opět počítat ciferný součet, ale v takzvaných
 * vyvážených ciferných soustavách, které mají jak záporné tak
 * kladné číslice. Budeme uvažovat pouze liché základy symetricky
 * rozložené kolem nuly (tzn. trojkovou s číslicemi ⟦-1, 0, 1⟧,
 * pětkovou ⟦-2, -1, 0, 1, 2⟧, atd.). Vaším úkolem je napsat
 * predikát ‹is_balanced›, který rozhodne, má-li zadané číslo ‹n› ve
 * vyvážené soustavě zadané svým základem ‹base› nulový ciferný
 * součet.
 *
 * Výpočet cifer čísla ⟦n⟧ ve vyvážené soustavě o základu ⟦b⟧
 * probíhá podobně, jako v té klasické se stejným základem. Nejprve
 * si připomeneme klasický algoritmus. Nastavíme ⟦n₀ = n⟧ a
 * opakujeme:
 *
 *  1. cifru ⟦cᵢ⟧ získáme jako zbytek po dělení ⟦nᵢ⟧ základem ⟦b⟧,
 *  2. spočítáme ⟦nᵢ₊₁⟧ tak, že vydělíme ⟦nᵢ⟧ základem ⟦b⟧,
 *  3. je-li výsledek nenulový, pokračujeme bodem 1, jinak skončíme.
 *
 * Abychom získali vyvážený zápis místo toho klasického, musíme
 * vyřešit situaci, kdy ⟦cᵢ⟧ není povolenou číslicí. Všimneme si, že
 * musí po každém kroku platit (přímo z definice použitých operací):
 *
 *  ⟦ nᵢ = cᵢ + bnᵢ₊₁ ⟧
 *
 * Tuto rovnost musíme zachovat, ale zároveň potřebujeme, aby ⟦cᵢ⟧
 * bylo platnou číslicí. To zajistíme jednoduše tak, že od ⟦cᵢ⟧
 * odečteme ⟦b⟧ a přičteme místo toho jedničku k ⟦nᵢ₊₁⟧ (tím se
 * součet jistě nezmění, protože jsme jedno ⟦b⟧ ubrali a jedno
 * přidali). */

bool is_balanced( int n, int base );

int main()
{
    assert( is_balanced( 8, 3 ) );
    assert( !is_balanced( 7, 3 ) );
    assert( is_balanced( 0, 5 ) );
    assert( is_balanced( 24, 5 ) );
    assert( is_balanced( 48, 5 ) );
    assert( !is_balanced( 1, 15 ) );

    return 0;
}
