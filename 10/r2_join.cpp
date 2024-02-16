#include <cassert>
#include <vector>
#include <array>

/* Naprogramujte generickou proceduru ‹join›, která bude mít 5
 * parametrů:
 *
 *  • dvě relace ⟦A, B⟧ na ⟦n⟧ resp. ⟦m⟧ hodnotách typu ‹T›,
 *    reprezentované jako ‹std::vector› hodnot typu ‹std::array›,
 *  • číslo ⟦j < n⟧ určující sloupec (index) v první relaci a číslo
 *    ⟦k < m⟧ podobně určující sloupec ve druhé relaci,
 *  • výstupní vektor vhodného typu, do kterého zapíše výsledek.
 *
 * Procedura nesmí vstupní parametry (zejména vstupní relace) nijak
 * měnit. Hodnoty v relacích lze kopírovat a implicitně sestrojovat.
 *
 * Výsledkem bude jediná relace ⟦R⟧ na ⟦n + m - 1⟧ hodnotách taková,
 * že:
 *
 *  • ⟦(a₀, …, aⱼ, …, aₙ, b₀, …, bₖ₋₁, bₖ₊₁, …, bₘ) ∈ R⟧ právě když
 *  • ⟦(a₀, …, aₙ) ∈ A⟧ a zároveň
 *  • ⟦(b₀, …, bₖ₋₁, aⱼ, bₖ₊₁, … bₘ) ∈ B⟧.
 *
 * V relačních databázových systémech se této operaci říká přirozené
 * spojení (za předpokladu, že ⟦i, j⟧ označují jediný společný
 * sloupec). */

int main()
{
    std::array a{ 1, 1 },
               b{ 1, 2 },
               c{ 3, 2 };
    std::array x{ 1, 3, 1 },
               y{ 1, 2, 1 },
               z{ 1, 1, 3 };

    std::vector rel_a{ a, b, c };
    std::vector rel_b{ x, y, z };

    using out_t = std::vector< std::array< int, 4 > >;
    out_t out;

    join( rel_a, rel_b, 1, 1, out );
    std::sort( out.begin(), out.end() );
    assert(( out == out_t{ { 1, 1, 1, 3 },
                           { 1, 2, 1, 1 },
                           { 3, 2, 1, 1 } } ));
    out.clear();

    join( rel_a, rel_b, 0, 2, out );
    std::sort( out.begin(), out.end() );
    assert(( out == out_t{ { 1, 1, 1, 2 },
                           { 1, 1, 1, 3 },
                           { 1, 2, 1, 2 },
                           { 1, 2, 1, 3 },
                           { 3, 2, 1, 1 } } ));

    return 0;
}
