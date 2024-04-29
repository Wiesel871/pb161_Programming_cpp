#include <cassert>
#include <cstdint>
#include <ostream>
#include <string_view>
#include <iostream>

/* V tomto cvičení implementujeme čísla s pevnou desetinnou čárkou,
 * konkrétně se 6 desítkovými číslicemi před a dvěma za desetinnou
 * čárkou. Čísla budou tedy tvaru ‹123456.78›. */

/* Typ ‹bad_format› budeme používat jako výjimku, která indikuje, že
 * pokus o načtení čísla z řetězce selhalo. */

struct bad_format {};

/* Typ ‹fixnum› nechť poskytuje tyto operace:
 *
 *  • sčítání, odečítání a násobení (operátory ‹+›, ‹-› a ‹*›),
 *  • sestrojení z celého čísla,
 *  • sestrojení z řetězce zadaného v desítkové soustavě (desetinná
 *    část je nepovinná) – je-li řetězec neplatný, konstruktor
 *    nechť skončí výjimkou ‹bad_format›,
 *  • srovnání dvou hodnot operátory ‹==› a ‹!=›.
 *
 * Všechny aritmetické operace nechť zaokrouhlují směrem k nule na
 * nejbližší reprezentovatelné číslo. */
template<typename T>
concept I32Compatible = requires(T t) {
    static_cast<std::int32_t>(t);
};

struct fixnum {
    std::int32_t inner;

    consteval fixnum() : inner(0) {}

    template<I32Compatible I32>
    constexpr fixnum(I32 num) : inner(num * 100) {
        //std::cout << "num: " << num << " " << inner << std::endl;
    }

    fixnum(std::string_view str) {
        if (str.empty())
            throw bad_format{};
        inner = 0;
        struct minus {
            bool val;
            int32_t &in;

            minus(bool b, int32_t &in) : val(b), in(in) {}

            operator bool() const {
                return val;
            }

            ~minus() {
                if (val)
                    in *= -1;
            }
        };
        auto dot = str.find('.');
        minus m(str[0] == '-', inner);
        for (std::size_t i = 0 + m; i < dot && i < str.size(); ++i) {
            if (str[i] < '0' || str[i] > '9')
                throw bad_format{};
            inner *= 10;
            inner += str[i] - '0';
        }
        inner *= 100;
        if (dot == str.npos)
            return;
        if (str[dot + 1] < '0' || str[dot + 1] > '9')
            throw bad_format{};
        inner += 10 * (str[dot + 1] - '0');
        if (dot + 2 == str.size())
            return;
        if (str[dot + 2] < '0' || str[dot + 2] > '9')
            throw bad_format{};
        inner += str[dot + 2] - '0';
//        std::cout << "string: " << str << " " << inner << std::endl;
    }

    constexpr fixnum operator+(const fixnum &r) const {
        fixnum res;
        res.inner = inner + r.inner;
        return res;
    }

    constexpr fixnum operator-(const fixnum &r) const {
        fixnum res;
        res.inner = inner - r.inner;
        return res;
    }

    constexpr fixnum operator*(const fixnum &r) const {
        fixnum res;
        res.inner = (inner * r.inner) / 100;
        return res;
    }

    constexpr bool operator==(const fixnum &r) const = default;

    constexpr auto operator<=>(const fixnum &r) const = default;
};

int main()
{
    assert( fixnum( 10 ) == fixnum( "10" ) );
    assert( fixnum( 1 )  == fixnum( "1" ) );

    fixnum a( 10 ), b( "0.5" ), c( "5.5" );
    const fixnum d( "0.01" ), e( "0.05" );

    assert( a + b + b == fixnum( 11 ) );
    assert( c + b == fixnum( 6 ) );
    assert( c + c - fixnum( 1 ) == a );
    assert( a * a * d == fixnum( 1 ) );
    assert( fixnum( 5 ) * d == e );
    assert( a * b == fixnum( 5 ) );
    assert( d * d == fixnum( 0 ) );
    assert( a * a == fixnum( 100 ) );
    assert( e * fixnum( 100 ) == fixnum( 5 ) );
    assert( e * c == fixnum( "0.27" ) );
    assert( fixnum( -1 ) * e * c == fixnum( -1 ) * fixnum( "0.27" ) );
    assert( fixnum( "123456.78" ) - fixnum( "123456.00" ) == fixnum( "0.78" ) );

    fixnum x( 0 );

    for ( int i = 0; i < 1000; ++i )
        for ( fixnum j : { d, d, fixnum( -2 ) * d } )
            for ( int k = 0; k < 1000000; ++k )
                x = x + j;


    for ( int i = 0; i < 1000; ++i )
    {
        assert( x * fixnum( i ) == fixnum( 0 ) );
        assert( !( x * fixnum( i ) != fixnum( 0 ) ) );
        assert( x * fixnum( i ) != fixnum( "0.01" ) );
        assert( !( x * fixnum( i ) == fixnum( "0.01" ) ) );
    }

    using badfmt = const bad_format &;

    try { fixnum( "x" );   assert( false ); } catch ( badfmt ) {}
    try { fixnum( "x.1" ); assert( false ); } catch ( badfmt ) {}
    try { fixnum( "1.." ); assert( false ); } catch ( badfmt ) {}
    try { fixnum( "1.x" ); assert( false ); } catch ( badfmt ) {}
    try { fixnum( "1,1" ); assert( false ); } catch ( badfmt ) {}

    return 0;
}
