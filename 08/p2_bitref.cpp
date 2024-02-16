#include <cassert>
#include <cstddef>

/* Navrhněte typ ‹bitref›, který se bude co nejvíce podobat
 * (nekonstantní) referenci na hodnotu typu ‹bool›, ale bude
 * „odkazovat“ jediný bit. Bity číslujeme od toho nejméně
 * významného (který má index 0). Je-li
 *
 *  • ‹br› hodnota typu ‹bitref›,
 *  • ‹b› hodnota typu ‹bool›,
 *  • ‹f› je funkce, která akceptuje parametr typu ‹bool›,
 *  • ‹off› hodnota typu ‹int› v rozsahu 0–7 a konečně,
 *  • ‹ptr› je dereferencovatelná hodnota typu ‹std::byte *›,
 *
 * tyto výrazy musí být dobře utvořené:
 *
 *  • ‹bitref( ptr, off )› – vytvoří „bitovou referenci“ na ‹off›-tý
 *    bit bajtu, na který ukazuje ‹ptr›,
 *  • ‹br = b› – nastaví odkazovaný bit na hodnotu ‹b›,
 *  • ‹!br›,
 *  • ‹br & b›, ‹b & br›, ‹br & br›,
 *  • podobně pro ostatní binární operátory: ‹|›, ‹&›, ‹^›, ‹+›,
 *    ‹-›, ‹*›, ‹/›, ‹%›,
 *  • ‹br += b›, ‹br += br›, podobně ‹*=›, ‹/=›, ‹&=›, ‹^=›, ‹|=›,
 *  • ‹br == b›, ‹b == br›, ‹br == br›,
 *  • podobně ostatní relační operátory: ‹<=›, ‹>=›, ‹<›, ‹>›, ‹!=›,
 *  • ‹br++›, ‹br--›, ‹++br›, ‹--br›,
 *  • ‹br && b›, ‹b && br›, ‹br && br›,
 *  • ‹br || b›, ‹b || br›, ‹br || br›,
 *  • ‹f( br )› – zavolá funkci ‹f› s hodnotou odkazovaného bitu
 *    jako parametrem.
 *
 * Navíc musí být možné použít ‹br› jako podmínku příkazů ‹if›,
 * ‹while›, ‹for›. */

struct bitref;

int main()
{
    std::byte a{ 0 }, b{ 1 };
    bitref a0( &a, 0 ),
           a7( &a, 7 ),
           b0( &b, 0 ),
           b3( &b, 3 );

    assert( b0 );
    assert( !b3 );

    a0 = true;
    assert( a == std::byte{ 1 } );
    a7 = true;
    assert( a == std::byte{ 129 } );
    a0 = false;
    assert( a == std::byte{ 128 } );

    assert( a0 | a7 );
    assert( !( a0 & a7 ) );
    assert( b3 ^ a7 );
    assert( true ^ a0 );
    assert( a7 ^ false );

    bitref t( &a, 7 ),
           f( &a, 0 );

    assert( t - f == t );
    assert( t - t == f );
    assert( f + t == t );
    assert( t / t == t );
    assert( t % t == f );
    assert( t * t == t );
    assert( t * f == f );
    assert( t == true );
    assert( true == t );
    assert( f != t );
    assert( f != true );
    assert( t <= t );
    assert( t >= true );
    assert( false < t );
    assert( f < true );

    assert( t += f );
    assert( t += false );
    assert( t -= f );
    assert( t -= false );
    assert( t *= t );
    assert( t *= true );
    assert( t /= t );
    assert( t /= true );
    assert( !( f %= t ) );
    assert( !( f %= true ) );
    assert( t &= t );
    assert( t &= true );
    assert( t |= f );
    assert( t |= false );
    assert( t ^= f );
    assert( t ^= false );
    assert( t );
    assert( !f );

    assert( a0 || a7 );
    assert( a0 || true );
    assert( a7 && a7 );

    auto neg = []( bool b ) { return !b; };
    assert( neg( a0 ) );

    return 0;
}
