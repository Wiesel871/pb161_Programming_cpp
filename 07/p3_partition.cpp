#include <cassert>
#include <vector>
#include <tuple>

/* Napište generický podprogram ‹partition( seq, p )›, který
 * přeuspořádá zadanou sekvenci tak, že všechny prvky menší než ‹p›
 * budou předcházet všem prvkům rovným ‹p› budou předcházet všem
 * prvkům větším než ‹p›. Sekvence ‹seq› má tyto metody:
 *
 *  • ‹size()› vrátí počet prvků uložených v sekvenci,
 *  • ‹swap( i, j )› prohodí prvky na indexech ‹i› a ‹j›,
 *  • ‹compare( i, p )› srovná prvek na pozici ‹i› s hodnotou ‹p›:
 *    ◦ výsledek -1 znamená, že hodnota na indexu ‹i› je menší,
 *    ◦ výsledek 0, že je stejná, a konečně
 *    ◦ výsledek +1, že je větší než ‹p›.
 *
 * Metoda ‹compare› může skončit výjimkou. V takovém případě vraťte
 * sekvenci ‹seq› do původního stavu a výjimku propagujte dál směrem
 * k volajícímu. Hodnoty typu ‹seq› nelze kopírovat, máte ale
 * povoleno použít pro výpočet dodatečnou paměť. Metody ‹size› ani
 * ‹swap› výjimkou skončit nemohou. */

int main()
{
    struct sequence
    {
        std::size_t size() const noexcept { return data.size(); }

        void swap( int i, int j ) noexcept
        {
            std::swap( data[ i ], data[ j ] );
        }

        int compare( std::size_t i, int p )
        {
            int value = data[ i ];

            if ( value % 3 == 0 )
                throw std::exception();

            return value < p ? -1 : value == p ? 0 : 1;
        }

        sequence( std::vector< int > d ) : data( d ) {}
        auto &operator=( const sequence & ) = delete;

    private:
        std::vector< int > data;
    };

    auto check = []( const std::vector< int > &values, int pivot )
    {
        sequence seq( values );
        partition( seq, pivot );

        std::size_t i = 0;
        auto size = seq.size();

        while ( i < size && seq.compare( i, pivot ) == -1 ) ++i;
        while ( i < size && seq.compare( i, pivot ) == 0 ) ++i;
        while ( i < size && seq.compare( i, pivot ) == 1 ) ++i;

        assert( i == size );
    };

    auto check_exception = []( const std::vector< int > &values, int pivot )
    {
        bool thrown = false;
        sequence seq( values );

        try
        {
            partition( seq, pivot );
        }
        catch( std::exception & )
        {
            thrown = true;
        }

        assert( thrown );

        for ( unsigned i = 0; i < values.size(); ++i )
            if ( values[ i ] != 3 )
                assert( seq.compare( i, values[ i ] ) == 0 );
    };

    check( { 2, 4, 1 }, 3 );
    check( { 2, 1, 5, 8, 4, 7, 5, 2, 5 }, 5 );

    check_exception( { 3, 2, 5, 1, 4, 2, 7, 8, 2, 1 }, 3 );
    check_exception( { 7, 2, 5, 1, 4, 2, 7, 8, 3, 1 }, 3 );

    return 0;
}
