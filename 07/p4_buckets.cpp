#include <cassert>
#include <vector>
#include <stdexcept>

/* Napište generický podprogram ‹buckets( list, vec )›, který
 * dostane na vstupu:
 *
 *  • referenci na seznam tokenů ‹list›, který má tyto metody:
 *    ◦ ‹front()› – vrátí referenci na první token seznamu,
 *    ◦ ‹drop()› – odstraní první token v seznamu,
 *    ◦ ‹empty()› – vrátí ‹true› je-li seznam prázdný,
 *  • referenci na hodnotu ‹vec› neurčeného typu, který má metodu
 *    ‹size()›, a který lze indexovat celými čísly. Uvnitř ‹vec›
 *    jsou uloženy kontejnery typu, který poskytuje metodu ‹emplace›,
 *    která vloží prvek kopií nebo přesunem (podle typu parametru),
 *    a metodu ‹pop›, která odstraní posledně vložený prvek a vrátí
 *    ho hodnotou (provede přesun z kontejneru).
 *
 * Tokeny nelze kopírovat ani přiřazovat kopií, pouze přesouvat.
 *
 * Podprogram bude ze vstupního seznamu ‹list› postupně odebírat
 * tokeny a bude je vkládat do ‹vec[ i % vec.size() ]›, kde ‹i› je
 * pořadové číslo odebraného tokenu v původním seznamu ‹list›
 * (počínaje nulou).
 *
 * Zabezpečte, aby byl výsledek konzistentní, a to i v případě, kdy
 * dojde k výjimce. Selhat mohou tyto operace (a žádné jiné):
 *
 *  • alokace paměti v metodě ‹emplace› (v takovém případě není
 *    prvek vložen, ani není zavolán jeho konstruktor),
 *  • metoda ‹drop› (odstranění prvku v tomto případě neproběhne).
 *
 * Zejména se nesmí žádný token ztratit, ani nesmí nikde zůstat
 * přebytečný vykradený (moved from) token. */

int main()
{
    struct test_list;
    struct test_subvec;

    struct token
    {
        token( int v ) : value( v ) {}
        token( const token &o ) = delete;

        token( token &&o ) noexcept
            : value( o.value )
        {
            o.value = 0;
        }

        token &operator=( const token &o ) = delete;
        token &operator=( token &&o ) noexcept
        {
            value = o.value;

            if ( &o != this )
                o.value = 0;

            return *this;
        }

        friend struct test_subvec;
        friend struct test_list;

    private:
        int value;
    };

    using int_vec = std::vector< int >;
    using vec_vec = std::vector< int_vec >;

    struct test_subvec
    {
        void emplace( token &&t )
        {
            if ( t.value % 3 == 0 )
                throw std::bad_alloc();

            _data.push_back( t.value );
            t.value = 0;
        }

        token pop() noexcept
        {
            token r( _data.back() );
            _data.pop_back();
            return r;
        }

        test_subvec( const int_vec &v ) : _data( v ), _prefill( v.size() ) {}

        bool _check( const int_vec &expect ) const
        {
            if ( _data.size() < expect.size() )
                return false;

            for ( unsigned i = 0; i < expect.size(); ++i )
                if ( _data[ i ] != expect[ i ] )
                    return false;

            return true;
        }

        friend struct test_list;

    private:
        std::vector< int > _data;
        std::size_t _prefill;
    };

    struct test_vec
    {
        auto size() const { return _data.size(); }

        const test_subvec &operator[]( std::size_t i ) const { return _data[ i ]; }
        test_subvec &operator[]( std::size_t i ) { return _data[ i ]; }

        test_vec( const vec_vec &vv )
        {
            for ( auto &v : vv )
                _data.emplace_back( v );
        }

        bool _check( const vec_vec &expect )
        {
            if ( _data.size() != expect.size() )
                return false;

            for ( std::size_t i = 0; i < _data.size(); ++i )
                if ( !_data[ i ]._check( expect[ i ] ) )
                    return false;

            return true;
        }

        friend struct test_list;

    private:
        std::vector< test_subvec > _data;
    };

    struct test_list
    {
        bool empty() const { return _data.empty(); }
        token &front() { return _data.back(); }

        void drop()
        {
            if ( _remains.back() == 14 )
                throw std::runtime_error( "no dice" );

            _dropped.push_back( _remains.back() );
            _remains.pop_back();
            _data.pop_back(); /* does not throw */
        }

        test_list( const int_vec &v )
        {
            for ( auto it = v.rbegin(); it != v.rend(); ++it )
            {
                _data.emplace_back( *it );
                _remains.push_back( *it );
            }
        }

        bool _check( const test_vec &tv ) const
        {
            for ( unsigned i = 0; i < _dropped.size(); ++i )
            {
                auto &tsv = tv[ i % tv.size() ];
                unsigned idx = tsv._prefill + i / tv.size();

                if ( tsv._data.size() <= idx )
                    return false;

                if ( i >= _dropped.size() - tv.size() )
                    if ( tsv._data.size() > idx + 1 )
                        return false;

                if ( tsv._data[ idx ] != _dropped[ i ] )
                    return false;
            }

            for ( unsigned i = 0; i < _remains.size(); ++i )
                if ( _data[ i ].value != _remains[ i ] )
                    return false;

            return true;
        }

    private:
        std::vector< token > _data;
        std::vector< int > _dropped;
        std::vector< int > _remains;
    };

    auto check = []( int_vec l, vec_vec v, vec_vec ve, bool throws )
    {
        test_list tl( l );
        test_vec tv( v );

        try
        {
            buckets( tl, tv );
            assert( !throws );
        } catch ( ... ) {}

        assert( tl._check( tv ) );
        assert( tv._check( ve ) );
    };

    check( { 1, 2, 8, 5, 4 }, { {}, {}, {} },
           { { 1, 5 }, { 2, 4 }, { 8 } }, false );
    check( { 1, 2, 4, 5, 7, 8 }, { { 3, 4 }, { 8 } },
           { { 3, 4, 1, 4, 7 }, { 8, 2, 5, 8 } }, false );

    check( { 1, 2, 3, 4, 5 }, { {}, {} }, { { 1 }, { 2 } }, true );
    check( { 1, 2, 2, 4, 5, 6, 6, 3 }, { { 5, 3 }, {}, { 4 }, { 8, 0, 0 } },
           { { 5, 3, 1, 5 }, { 2 }, { 4, 2 }, { 8, 0, 0, 4 } }, true );

    check( { 1, 13, 14, 2 }, { {}, {} }, { { 1 }, { 13 } }, true );

    return 0;
}
