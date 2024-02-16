#include <string>
#include <cassert>
#include <stdexcept>

/* V tomto cvičení se vrátíme k oblíbenému vyhledávacímu algoritmu
 * půlením intervalu. Budeme implementovat kontejner, který se
 * podobá na ‹std::map›, ale budeme předpokládat, že vyhledávání je
 * mnohem častější než vkládání. Proto budeme preferovat strukturu,
 * kde je vyhledávání podle možnosti co nejrychlejší, i za cenu
 * pomalejšího vkládání.
 *
 * Vhodným kandidátem je seřazené pole¹ – hledání je logaritmické
 * podobně jako u vyhledávacího stromu, ale data jsou uložena mnohem
 * kompaktněji a proto je i práce s nimi výrazně rychlejší.
 *
 * Implementujte metody ‹emplace›, ‹at› a ‹contains›, s chováním,
 * které odpovídá typu ‹std::map›, s výjimkou ‹emplace›, který vrátí
 * pouze hodnotu typu ‹bool› (iterátory implementovat nebudeme).
 *
 * Konečně, protože neumíme psát generické třídy, klíče i hodnoty
 * budou pevných typů: ‹int› pro klíče a ‹token› pro hodnoty. */

struct token
{
    token( int i ) : _value( i ) {}
    token( const token & ) = delete;
    token( token && ) = default;

    token &operator=( const token & ) = delete;
    token &operator=( token && ) = default;
    token &operator=( int v ) { _value = v; return *this; }

    bool operator==( int v ) const { return _value == v; }

private:
    int _value;
};

class flat_map;

/* ¹ Požadujeme, aby kontejner ‹flat_map› po libovolné sekvenci
 *   operací používal nejvýše dvě souvislé dynamicky alokované
 *   oblasti paměti. Kontejner ‹std::vector› používá nejvýše jednu. */

int main()
{
    flat_map fm;
    const flat_map &cfm = fm;

    std::map< int, token > x;
    x.emplace( 7, 7 );

    assert(  fm.emplace( 1, 3 ) );
    assert(  fm.emplace( -7, 7 ) );
    assert( !fm.emplace( -7, 3 ) );

    assert( cfm.contains( -7 ) );
    assert( cfm.at( -7 ) == 7 );

    try
    {
        fm.at( -2 );
        assert( 0 );
    }
    catch ( const std::out_of_range & ) {};

    fm.at( -7 ) = 4;
    assert( fm.at( -7 ) == 4 );

    assert(  fm.emplace( -8, 2 ) );
    assert( !fm.emplace( -7, 3 ) );

    const token &bar = cfm.at( -7 );
    assert( bar == 4 );

    return 0;
}
