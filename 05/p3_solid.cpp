#include <cassert>
#include <vector>

/* V tomto cvičení se zaměříme na typy (v tomto cvičení typ ‹solid›)
 * s volitelnými složkami (typ ‹transform_matrix›). Budou nás
 * zejména zajímat situace, kdy je relativně častý případ, že
 * volitelná data nejsou potřebná, a zároveň jsou dostatečně velká
 * aby mělo smysl je oddělit do samostatného objektu (v samostatně
 * alokované oblasti paměti). Zároveň budeme požadovat, aby
 * «logicky» hodnoty hlavního typu (‹solid›) vystupovaly jako jeden
 * celek a nepřítomnost volitelných dat byla vnějšímu světu podle
 * možnosti skrytá. */

/* Typ ‹solid› bude reprezentovat nějaké třírozměrné těleso, zatímco
 * typ ‹transform_matrix› bude popisovat třírozměrnou lineární
 * transformaci takového tělesa, a bude tedy reprezentován devíti
 * čísly s plovoucí desetinnou čárkou (3 řádky × 3 sloupce). Tyto
 * hodnoty nechť jsou (přímo nebo nepřímo) položkami typu
 * ‹transform_matrix› (bez jakékoliv další pomocné paměti).
 * Implicitně sestrojená hodnota nechť reprezentuje identitu
 * (hodnoty na hlavní diagonále rovné 1, mimo diagonálu 0). */

struct transform_matrix;

/* Typ ‹solid› bude reprezentovat společné vlastnosti pevných těles
 * (které nezávisí na konkrétním tvaru nebo typu tělesa). Měl by mít
 * tyto metody:
 *
 *  • ‹pos_x›, ‹pos_y› a ‹pos_z› určí polohu těžiště v prostoru,
 *  • ‹transform_entry( int r, int c )› udává koeficient
 *    transformační matice na řádku ‹r› a sloupci ‹c›,
 *  • ‹transform_set( int r, int c, double v )› nastaví příslušný
 *    koeficient na hodnotu ‹v›,
 *  • konstruktor přijme 3 parametry typu ‹double› (vlastní
 *    souřadnice ⟦x⟧, ⟦y⟧ a ⟦z⟧).
 *
 * Výchozí transformační maticí je opět identita. Paměť pro tuto
 * matici alokujte pouze v případě, že se oproti implicitnímu stavu
 * změní některý koeficient. */

struct solid;

int main()
{
    /* check that the classes do not contain any extraneous data */
    static_assert( sizeof( solid ) <= 3 * sizeof( double ) + sizeof( void * ) );
    static_assert( sizeof( transform_matrix ) == 9 * sizeof( double ) );

    auto check_mat = []( const auto &s, std::vector< double > v )
    {
        return s.transform_entry( 0, 0 ) == v[ 0 ] &&
               s.transform_entry( 0, 1 ) == v[ 1 ] &&
               s.transform_entry( 0, 2 ) == v[ 2 ] &&
               s.transform_entry( 1, 0 ) == v[ 3 ] &&
               s.transform_entry( 1, 1 ) == v[ 4 ] &&
               s.transform_entry( 1, 2 ) == v[ 5 ] &&
               s.transform_entry( 2, 0 ) == v[ 6 ] &&
               s.transform_entry( 2, 1 ) == v[ 7 ] &&
               s.transform_entry( 2, 2 ) == v[ 8 ];
    };

    solid s( 0, 1, 2 );
    const solid &cs = s;

    assert( cs.pos_x() == 0 );
    assert( cs.pos_y() == 1 );
    assert( cs.pos_z() == 2 );

    assert( check_mat( s, { 1, 0, 0,
                            0, 1, 0,
                            0, 0, 1 } ) );

    s.transform_set( 0, 1, 3 );
    s.transform_set( 2, 1, 3 );

    assert( check_mat( s, { 1, 3, 0,
                            0, 1, 0,
                            0, 3, 1 } ) );

    s.transform_set( 2, 2, 5 );

    assert( check_mat( s, { 1, 3, 0,
                            0, 1, 0,
                            0, 3, 5 } ) );

    return 0;
}
