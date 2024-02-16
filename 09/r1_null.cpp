#include <cassert>
#include <optional>
#include <functional>
#include <vector>

/* V tomto cvičení se budeme zabývat kontejnery, ve kterých mohou
 * některé hodnoty chybět – takové hodnoty budeme reprezentovat
 * pomocí ‹std::nullopt›. V následovných funkcích nechť platí, že
 * výsledek operace, kde alespoň jeden operand je ‹std::nullopt› je
 * opět ‹std::nullopt›. Implementujte:
 *
 *  • ‹filter›, která ze zadané sekvence odstraní prázdné hodnoty,
 *  • ‹zip›, která dostane dvě posloupnosti a funkci, kterou po
 *    dvojicích aplikuje a tím vytvoří novou posloupnost (jako
 *    hodnotu typu ‹std::vector›),
 *  • ‹join›, která ze zadaných posloupností a binárního predikátu
 *    vytvoří posloupnost dvojic (kartézský součin), ale jen
 *    takových, které splňují zadaný predikát.
 *
 * Hodnotu ‹std::nullopt› interpretovanou jako pravdivostní hodnotu
 * chápeme jako ekvivalent ‹false›. */

int main()
{
    using ivec = std::vector< std::optional< int > >;
    using jvec = std::vector< std::tuple< int, int > >;

    const ivec x{ 3, std::nullopt, 4 },
               y{ 1, 2, std::nullopt },
               z{ 1, 1, 2, 3 };

    assert(( filter( x ) == std::vector{ 3, 4 } ));
    assert(( zip( x, x, std::plus() ) ==
             ivec{ 6, std::nullopt, 8 } ));
    assert(( zip( x, y, std::plus() ) ==
             ivec{ 4, std::nullopt, std::nullopt } ));

    assert(( join( x, y, std::greater() ) ==
             jvec{ { 3, 1 }, { 3, 2 }, { 4, 1 }, { 4, 2 } } ));
    assert(( join( y, z, std::equal_to() ) ==
             jvec{ { 1, 1 }, { 1, 1 }, { 2, 2 } } ));

    return 0;
}
