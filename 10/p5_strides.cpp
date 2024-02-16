#include <vector>
#include <cassert>
#include <algorithm>

/* Naprogramujte generickou proceduru ‹stride_sort( seq, key )›,
 * která seřadí vstupní posloupnost ‹seq› podle klíče zadaného
 * unární funkcí ‹key›, a to v čase O(⟦k⋅log(k) + n⟧), kde ⟦k⟧ je
 * počet již správně seřazených běhů.
 *
 * Běhy ve vstupní posloupnosti se «nepřekrývají», tzn. pro
 * libovolné dva běhy ⟦s⟧, ⟦t⟧ platí buď ⟦max(s) < min(t)⟧ nebo
 * naopak ⟦max(t) < min(s)⟧. Pro zcela seřazený vstup je ⟦k = 1⟧.
 *
 * Zvažte jak, a za jakou cenu, by šlo algoritmus zobecnit tak, aby
 * se vstupní běhy mohly překrývat (a zároveň zůstal pro malá ⟦k⟧
 * výrazně efektivnější než obecné řazení). */

int main()
{
    std::vector a{ 4, 5, 6, 1, 2, 3 };
    stride_sort( a, []( const auto &x ) { return x; } );
    assert( std::is_sorted( a.begin(), a.end() ) );

    struct token
    {
        int value;

        token( int v ) : value( v ) {}

        token( const token & ) = delete;
        token( token && ) = default;

        token &operator=( const token & ) = delete;
        token &operator=( token && ) = default;

        bool operator==( const token & ) const = default;
        auto operator<=>( const token & ) const = default;
    };

    struct weight
    {
        int value;
        weight( int v ) : value( v ) {}

        weight( const weight & ) = delete;
        weight( weight && ) = delete;

        weight &operator=( const weight & ) = delete;
        weight &operator=( weight && ) = delete;

        bool operator==( const weight &o ) const = default;
        auto operator<=>( const weight &o ) const = default;
    };

    int ops = 0;
    int count = 16 * 1024;
    int strides = 3;

    auto reversed = [&]( const auto &t )
    {
        ++ops;
        return weight( -t );
    };

    std::vector< int > x;

    for ( int i = 0; i < strides; ++i )
        for ( int j = 0; j < count; ++j )
            x.emplace_back( i * count - j );

    stride_sort( x, reversed );
    assert( std::is_sorted( x.begin(), x.end(), std::greater() ) );
    assert( ops <= 2 * strides * count + strides * strides );

    return 0;
}
