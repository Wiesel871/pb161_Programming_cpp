#include <cassert>

/* V této úloze se vrátíme k datové struktuře „zipper“ –
 * připomínáme, že tato struktura reprezentuje sekvenci prvků
 * (v našem případě celých čísel), přitom právě jeden z těchto prvků
 * je «aktivní» (angl. focused).
 *
 * Tentokrát budeme tuto strukturu reprezentovat pomocí dvojice
 * zřetězených seznamů sestavených z ukazatelů typu ‹unique_ptr›.
 * Seznamy začínají v aktivním prvku a pokračují každý na jeden
 * konec struktury. Typ ‹zipper› bude mít toto rozhraní:
 *
 *  • konstruktor vytvoří jednoprvkový ‹zipper› z celého čísla,
 *  • ‹shift_left› (‹shift_right›) aktivuje prvek vlevo (vpravo) od
 *    toho dosud aktivního, a to v čase O(1); metody vrací ‹true›
 *    bylo-li posun možné provést (jinak nic nezmění a vrátí
 *    ‹false›),
 *  • ‹insert_left› (‹insert_right›) přidá nový prvek těsně vlevo
 *    (vpravo) od právě aktivního prvku (opět v čase O(1))
 *  • ‹focus› zpřístupní aktivní prvek (pro čtení i zápis). */

int main()
{
    zipper a( 7 ); // [7]
    const zipper &c = a;

    assert( c.focus() == 7 ); /* constant read works */
    assert( !a.shift_left() );
    assert( c.focus() == 7 );
    assert( !a.shift_right() );
    assert( c.focus() == 7 );

    a.insert_left( 3 );        // 3 [7]
    assert( c.focus() == 7 );
    assert( a.shift_left() );  // [3] 7
    assert( c.focus() == 3 );
    assert( !a.shift_left() );
    assert( a.shift_right() ); // 3 [7]
    assert( c.focus() == 7 );
    assert( !a.shift_right() );

    a.insert_left( 2 );   // 3 2 [7]
    a.insert_right( 4 );  // 3 2 [7] 4
    a.insert_right( 11 ); // 3 2 [7] 11 4
    a.focus() = 8; /* assignment to focus() works */

    assert( c.focus() == 8 );
    assert( a.shift_right() ); // 3 2 8 [11] 4
    assert( c.focus() == 11 );
    assert( a.shift_right() ); // 3 2 8 11 [4]
    assert( !a.shift_right() );

    assert( a.shift_left() ); // 3 2 8 [11] 4
    assert( a.focus() == 11 );
    assert( a.shift_left() ); // 3 2 [8] 11 4
    assert( a.focus() == 8 );
    assert( a.shift_left() ); // 3 [2] 8 11 4
    assert( a.focus() == 2 );
    assert( a.shift_left() ); // [3] 2 8 11 4
    assert( a.focus() == 3 );
    assert( !a.shift_left() );

    return 0;
}
