class statement;
using state = std::map< char, int >;
using stmt_ptr = std::unique_ptr< statement >;

class statement
{
public:
    std::string print() const
    {
        brq::string_builder b;
        b << "\n";
        print( b, 0 );
        return b.buffer();
    }

    virtual void print( brq::string_builder &, int ) const = 0;

    state eval( state s, int counter ) const
    {
        update( s, counter );
        assert( counter >= 0 );
        return s;
    }

    virtual void update( state &s, int &ctr ) const = 0;
    virtual ~statement() = default;
};

class stmt_inc : public statement
{
    char _var;
public:
    stmt_inc( char v ) : _var( v ) {}

    void print( brq::string_builder &b, int i ) const override
    {
        b << brq::pad( i, ' ' ) << brq::mark << brq::rawchr( _var ) << "++\n";
    }

    void update( state &s, int &counter ) const override
    {
        if ( !counter )
            return;

        auto &v = s[ _var ];

        ++ v;
        v %= 16;
    }
};

class stmt_while : public statement
{
    char _v_1, _v_2;
    stmt_ptr _body;

public:
    stmt_while( char v1, char v2, stmt_ptr b )
        : _v_1( v1 ), _v_2( v2 ), _body( std::move( b ) )
    {}

    void print( brq::string_builder &b, int i ) const override
    {
        b << brq::pad( i, ' ' ) << brq::mark
          << "while " << brq::rawchr( _v_1 ) << " != " << brq::rawchr( _v_2 ) << "\n";
        _body->print( b, i + 2 );
    }

    void update( state &s, int &counter ) const override
    {
        while ( counter && --counter && s[ _v_1 ] != s[ _v_2 ] )
            _body->update( s, counter );
    }
};

class stmt_block : public statement
{
    std::vector< stmt_ptr > _body;

public:
    void append( stmt_ptr stmt )
    {
        _body.emplace_back( std::move( stmt ) );
    }

    void print( brq::string_builder &b, int i ) const override
    {
        for ( const auto &s : _body )
            s->print( b, i );
    }

    void update( state &s, int &ctr ) const override
    {
        for ( const auto &stmt : _body )
            stmt->update( s, ctr );
    }
};

