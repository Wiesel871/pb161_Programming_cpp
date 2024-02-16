/* Navrhněte typy ‹program› a ‹grid›, které budou reprezentovat
 * jednoduchého programovatelného robota, který se pohybuje
 * v neomezené dvourozměrné mřížce. Políčka v mřížce mají dva stavy:
 * označeno a neoznačeno. Na začátku jsou všechna políčka
 * neoznačena, robot stojí na souřadnicích ⟦0, 0⟧ a je orientován
 * horizontálně.
 *
 * Program lze rozšířit metodou ‹append›, která přijme jako parametr
 * libovolný typ příkazu. Sestavený program lze vykonat volnou
 * funkcí ‹run›, která dostane jako druhý parametr referenci na
 * hodnotu typu ‹grid›. Funkce ‹run› jednak upraví vstupní mřížku,
 * jednak vrátí koncové souřadnice robota. */

/* Příkaz ‹walk› robota posune o příslušný počet políček podle
 * aktuální orientace:
 *
 *  • horizontální – kladná čísla znamenají východ,
 *  • vertikální – kladná čísla znamenají sever.
 *
 * Která vzdálenost se použije závisí na tom, je-li startovní
 * políčko označené. */

struct walk
{
    int if_marked = 0;
    int if_unmarked = 0;
};

/* Příkaz ‹turn› robota přepíná mezi horizontálním a vertikálním
 * směrem pohybu. */

struct turn {};

/* Příkaz ‹toggle› změní označenost políčka, na kterém robot
 * aktuálně stojí. Je-li příznak ‹sticky› nastaven na ‹true›, již
 * označené políčko zůstane označené. */

struct toggle
{
    bool sticky = false;
};

struct program;
struct grid;

std::tuple< int, int > run( const program &, grid & );

int main()
{
    program p, q;
    p.append( turn{} );
    p.append( walk{ 1, 2 } );
    p.append( toggle{} );

    grid g;
    assert( run( p, g ) == std::tuple( 0, 2 ) );
    assert( run( p, g ) == std::tuple( 1, 2 ) );

    p.append( toggle{} );
    assert( run( p, g ) == std::tuple( 1, 3 ) );
    assert( run( p, g ) == std::tuple( 3, 3 ) );

    q.append( walk{ -1, 1 } );
    q.append( toggle{} );
    assert( run( q, g ) == std::tuple( 4, 3 ) );
    assert( run( q, g ) == std::tuple( 3, 3 ) );

    return 0;
}
