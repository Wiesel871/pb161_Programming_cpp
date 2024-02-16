#include <cassert>

/* Vaší úlohou je naprogramovat jednoduchý simulátor hry, kde hráč
 * ovládá létající objekt („loď“) v bočním pohledu. Cílem hráče je
 * nenarazit do hranic „jeskyně“ ve které se pohybuje, a která je
 * zadaná jako seznam dvojic, kde čísla na indexu ⟦i⟧ udávají vždy
 * souřadnice ⟦y⟧ spodní a horní meze příslušného sloupce – pole –
 * v rozsahu souřadnice ⟦x ∈ ⟨i, i + 1)⟧. Například:
 *
 *   ╶───┬┬┬┬┬┬─┬┬┬┬┬┬┬┬┬┬─┬┬──╴
 *       └┤││├┘ └┤│││││├┴┘ └┘
 *        └┤├┘   └┴┴┤│├┘
 *    ●▶   └┘       └┴┘   ┌┐
 *    ┌┐      ┌┐         ┌┤│
 *    │├┐    ┌┤├┐ ┌┬┬┬┬┬┬┤│├┐
 *   ╶┴┴┴────┴┴┴┴─┴┴┴┴┴┴┴┴┴┴┴──╴
 *
 * Loď lze ovládat nastavením stoupání ⟦c⟧ (pro každý posun o ⟦l⟧
 * jednotek doprava se loď zároveň posune ⟦c⋅l⟧ jednotek nahoru;
 * je-li ⟦c⟧ záporné, posouvá se dolů). Hra má tyto 4 metody:
 *
 *  • ‹append( y₁, y₂ )› přidá na pravý konec hracího pole novou
 *    dvojici překážek (zadanou čísly s plovoucí desetinnou čárkou),
 *  • ‹move( l )› posune loď o ‹l› jednotek doprava (‹l› je celé
 *    číslo; při posunu dojde také k příslušné změně výšky podle
 *    aktuálního nastavení) a vrátí ‹true› v případě, že při tomto
 *    posunu nedošlo ke kolizi,
 *  • ‹set_climb( c )› nastaví aktuální stoupání na ‹c› (číslo
 *    s plovoucí desetinnou čárkou),
 *  • ‹finished()› vrátí ‹true› nachází-li se loď na pravém konci
 *    hracího pole.
 *
 * V případě, že dojde k pokusu o posun lodě za konec pole, loď
 * zůstane na posledním definovaném poli. Dojde-li ke kolizi, další
 * volání ‹move› již nemají žádný efekt a vrací ‹false›.
 *
 * Implicitně sestrojený stav hry má hrací pole délky 1 s překážkami
 * ⟦(-10, +10)⟧ a počáteční výška i stoupání lodě jsou 0. */

struct flight;

int main()
{
    flight f, g, h;

    assert( f.finished() );

    f.append( -7,  10 );
    f.append( -8,  10 );
    f.append( -10,  8 );
    f.append( -10,  6 );
    f.append( -10, -1 );

    assert( !f.finished() );

    assert(  f.move( 4 ) );
    assert( !f.move( 1 ) );

    for ( int i = 0; i < 20; ++i )
        g.append( -10, 10 );

    g.set_climb( 1.5 );

    assert(  g.move( 6 ) );
    assert( !g.move( 1 ) );

    for ( int i = 0; i < 20; ++i )
        h.append( -10, 10 );

    assert( h.move( 50 ) );
    assert( h.finished() );

    return 0;
}
