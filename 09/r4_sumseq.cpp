/* V tomto cvičení budeme programovat funkce, které pracují se
 * součtovými posloupnostmi, které mohou obsahovat prvky dvou typů
 * (budeme je označovat jako levý a pravý). Konkrétní reprezentace
 * takové posloupnosti není určena – musí být pouze kompatibilní
 * mezi jednotlivými funkcemi. */

/* První funkcí, kterou naprogramujeme, bude ‹select› – má 3
 * parametry, 2 obyčejné posloupnosti (obecně různých typů) a
 * binární funkci ‹choose›.
 *
 * Výsledkem bude součtová posloupnost, kde na každé pozici bude
 * hodnota ze stejné pozice některé vstupní posloupnosti – která
 * to bude určí funkce ‹choose›, které návratová hodonta je typu
 * ‹choice›: */

enum class choice { left, right };

// …

/* Dále definujeme funkce ‹left› a ‹right›, které ze zadané
 * „součtové“ posloupnosti vyberou prvky pouze levého (pravého) typu
 * a vrátí je jako obyčejnou posloupnost (reprezentovanou jako
 * ‹std::vector›). */

// …

/* Konečně definujeme funkci ‹map›, která obdrží součtovou
 * posloupnost a dvě funkce, které zobrazí hodnoty levého/pravého
 * typu na libovolný společný typ. Výsledkem je obyčejná posloupnost
 * vhodného typu (opět reprezentovaná jako ‹std::vector›). */

// …

int main()
{
    std::vector< int > a{ 1, 3, 4, 5 };
    std::vector< double > b{ 1.1, 2.0, 3.5, 5.1 };

    auto smaller = []( auto x, auto y )
    {
        return x <= y ? choice::left : choice::right;
    };

    auto small = select( a, b, smaller );

    assert(( left( small ) == std::vector{ 1, 5 } ));
    assert(( right( small ) == std::vector{ 2.0, 3.5 } ));

    auto id_int = []( int x ) { return x; };
    auto to_int = []( double x ) -> int { return x; };

    auto as_int = map( small, id_int, to_int );
    assert(( as_int == std::vector{ 1, 2, 3, 5 } ));

    return 0;
}
