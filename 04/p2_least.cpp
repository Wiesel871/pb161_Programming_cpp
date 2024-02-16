#include <cassert>
#include <memory>

/* Uvažme typ ‹element› hodnot, které (z nějakého důvodu) nelze
 * kopírovat. Našim cílem bude naprogramovat funkci, která vrátí
 * nejmenší prvek ze zadaného vektoru hodnot typu ‹element›.
 * Definici tohoto typu nijak neměňte. */

struct element
{
    element( int v ) : value( v ) {}
    element( element &&v ) : value( v.value ) {}
    element &operator=( element &&v ) = default;
    bool less_than( const element &o ) const { return value < o.value; }
    bool equal( const element &o ) const { return value == o.value; }
private:
    int value;
};

using data = std::vector< element >;

/* Naprogramujte funkci (nebo rodinu funkcí) ‹least› tak, že volání
 * ‹least( d )› vrátí nejmenší prvek zadaného vektoru ‹d› typu
 * ‹data›. Dobře si rozmyslete platnost (délku života) dotčených
 * objektů.
 *
 * Nápověda: Protože nemůžete přímo manipulovat hodnotami typu
 * ‹element›, zkuste využít k zapamatování si dosud nejlepšího
 * kandidáta iterátor. */

int main()
{
    auto mk = []( auto... vs )
    {
        data d;
        ( d.emplace_back( vs ), ... );
        return d;
    };

    data d_1 = mk( 1, 2, 3 ), d_2 = mk( 3, 2, 7 );
    assert( least( d_1 ).equal( 1 ) );
    assert( least( d_2 ).equal( 2 ) );
    const element &l = least( mk( 4, 3, 1, 2 ) );
    assert( l.equal( 1 ) );

    return 0;
}
