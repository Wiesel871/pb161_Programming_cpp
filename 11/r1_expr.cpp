/* Napište čistou funkci ‹expr_valid›, která rozhodne, je-li vstupní
 * řetězec výrazem, který vyhovuje následující gramatice:
 *
 *     expr   = ws, term, { ws, '+', term } ;
 *     term   = ws, factor, { ws, '*', factor } ;
 *     factor = ws, ( '(', expr, ws, ')' | ident | num ) ;
 *     ws     = { ? std::isspace ? } ;
 *     ident  = letter, { letter } ;
 *     num    = digit, { digit } ;
 *     letter = 'a' | 'b' | … | 'z' ;
 *     digit  = '0' | '1' | … | '9' ; */

bool expr_valid( std::string_view );

int main()
{
    assert( expr_valid( "0 + 0" ) );
    assert( expr_valid( "0 + 0 + 0" ) );
    assert( expr_valid( "0 + 0 * 0" ) );
    assert( expr_valid( "0 * 0 * 0" ) );
    assert( expr_valid( "(1 + 0) * 0" ) );
    assert( expr_valid( "( 1 + 0 ) * 0" ) );
    assert( expr_valid( "((1)+1)+1" ) );
    assert( expr_valid( "x + xy + 1" ) );

    assert( !expr_valid( "0 +" ) );
    assert( !expr_valid( "+ 0" ) );
    assert( !expr_valid( "(0 + 0" ) );
    assert( !expr_valid( "0 + 0)" ) );

    return 0;
}
