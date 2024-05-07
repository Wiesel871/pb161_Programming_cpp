#include <cassert>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <iostream>

/* V tomto příkladu se vrátíme k typu ‹fixnum› z předchozí kapitoly.
 * Jedná se o typ, který reprezentuje čísla s pevnou desetinnou
 * čárkou, konkrétně tvaru ‹123456.78›, se 6 desítkovými číslicemi
 * před a dvěma za desetinnou čárkou, a s těmito operacemi:
 *
 *  • sčítání, odečítání a násobení (operátory ‹+›, ‹-› a ‹*›),
 *  • sestrojení z celého čísla (implicitně nula),
 *  • přiřazení kopií,
 *  • srovnání dvou hodnot operátory ‹==› a ‹!=›,
 *  • čtení a zápis čísel z vstupně-výstupních proudů.
 *
 * Všechny aritmetické operace nechť zaokrouhlují směrem k nule na
 * nejbližší reprezentovatelné číslo. */
template<typename T>
concept I32Compatible = requires(T t) {
    static_cast<std::int32_t>(t);
};

struct bad_format {};

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
        if (dot == str.npos || dot + 1 == str.size())
            return;
        if (str[dot + 1] < '0' || str[dot + 1] > '9')
            throw bad_format{};
        inner += 10 * (str[dot + 1] - '0');
        if (dot + 2 == str.size())
            return;
        if (str[dot + 2] < '0' || str[dot + 2] > '9')
            throw bad_format{};
        inner += str[dot + 2] - '0';
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

    friend std::istream &operator>>(std::istream &is, fixnum &fn) {
        if (is.eof())
            return is;
        std::string num;
        auto pos = is.tellg();
        while (is && isspace(is.peek()))
            is.get();
        while (is && isdigit(is.peek())) {
            num.push_back(is.get());
        }
        int count = 0;
        if (is && is.peek() == '.') {
            num.push_back(is.get());
            while (is && isdigit(is.peek()) && count < 3) {
                count++;
                num.push_back(is.get());
            }
        }
        if (count == 3) {
            is.seekg(pos);
            is.setstate(std::istream::failbit);
        } else {
            fn = fixnum(num);
        }

        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const fixnum &fn) {
        int whole = fn.inner / 100;
        int dec = fn.inner % 100;
        return os << whole << "." << dec / 10 << dec % 10;
    }
};


int main()
{
    std::stringstream s( "10 1" );
    fixnum x, y;
    s >> x;
    std::clog << "s: '" << s.peek() << "'\n";
    s >> y;

    assert( fixnum( 10 ) == x );
    assert( fixnum( 1 )  == y );

    s.clear();
    s.str( "0.5 5.5 0.01 0.05 0.27" );

    fixnum a( 10 ), b, c, d, e, f;
    s >> b >> c >> d >> e >> f;

    s.clear();
    s.str( "" );
    s << a << " " << std::setw( 2 ) << 0;
    assert( s.str() == "10.00  0" );

    std::clog << a << '\n';
    std::clog << b << '\n';
    assert( a + b + b == fixnum( 11 ) );
    assert( c + b == fixnum( 6 ) );
    assert( c + c - fixnum( 1 ) == a );
    assert( a * a * d == fixnum( 1 ) );
    assert( fixnum( 5 ) * d == e );
    assert( a * b == fixnum( 5 ) );
    assert( d * d == fixnum( 0 ) );
    assert( a * a == fixnum( 100 ) );
    assert( e * fixnum( 100 ) == fixnum( 5 ) );
    assert( e * c == f );
    assert( fixnum( -1 ) * e * c == fixnum( -1 ) * f );

    s.clear();
    s.str( "123456.78 123456.00 0.78" );

    s >> b >> c >> e;

    assert( b - c == e );
    x = fixnum( 0 );

    for ( int i = 0; i < 1000; ++i )
        for ( fixnum j : { d, d, fixnum( -2 ) * d } )
            for ( int k = 0; k < 1000000; ++k )
                x = x + j;

    for ( int i = 0; i < 1000; ++i )
    {
        assert( x * fixnum( i ) == fixnum( 0 ) );
        assert( !( x * fixnum( i ) != fixnum( 0 ) ) );
        assert( x * fixnum( i ) != d );
        assert( !( x * fixnum( i ) == d ) );
    }

    s.clear();
    s.str( "123,x 1.1y 1.." );

    std::string sx, sy, se;
    s >> x >> sx >> y >> sy >> e >> se;
    assert( x == 123 );
    assert( sx == ",x" );
    assert( y == fixnum( 110 ) * d );
    assert( sy == "y" );
    assert( e == 1 );
    assert( se == "." );

    s.clear();
    s.str( "1.001" );
    s >> x;
    assert( !s );

    return 0;
}
