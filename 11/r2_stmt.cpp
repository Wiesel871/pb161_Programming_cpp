/* Napište čistou funkci ‹stmt_valid›, která rozhodne, je-li vstupní
 * řetězec příkazem jazyka zadaného následující gramatikou:
 *
 *     stmt = 'if', expr, 'then', stmt
 *          | 'while', expr, 'do', stmt
 *          | 'begin', stmt, { stmt }, 'end'
 *          | 'set', ident, ':=', expr
 *          | 'skip' ;
 *     expr = atom | atom '+' atom | atom '-' atom ;
 *     atom = '0' | '1' | ident ;
 *
 * Terminály (a neterminál ‹ident›) jsou od sebe na vstupu odděleny
 * právě jednou mezerou. Pro ‹ident› použijte tuto definici:
 *
 *     ident = letter, { letter } ;
 *     letter = 'a' | 'b' | … | 'z' ; */

bool stmt_valid( std::string_view );

int main()
{
    assert( stmt_valid( "skip" ) );
    assert( stmt_valid( "begin skip end" ) );
    assert( stmt_valid( "if 0 then skip" ) );
    assert( stmt_valid( "set x := 0" ) );
    assert( stmt_valid( "set x := x + 1" ) );
    assert( stmt_valid( "begin set x := 0 end" ) );
    assert( stmt_valid( "begin set x := 0 set y := 1 end" ) );
    assert( stmt_valid( "if 0 then begin skip end" ) );

    assert( !stmt_valid( "x" ) );
    assert( !stmt_valid( "x + 1" ) );
    assert( !stmt_valid( "begin end" ) );
    assert( !stmt_valid( "skip skip" ) );
    assert( !stmt_valid( "if x skip" ) );
    assert( !stmt_valid( "set x := x + 1 + 1" ) );
    assert( !stmt_valid( "begin set x := x + 1 + 1 end" ) );

    return 0;
}
