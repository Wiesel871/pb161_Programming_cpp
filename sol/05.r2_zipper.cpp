struct node
{
    using ptr = std::unique_ptr< node >;
    int value;
    ptr next;
    node( int v, ptr n ) : value( v ), next( std::move( n ) ) {}
};

class zipper
{
    int _focus;
    using node_ptr = std::unique_ptr< node >;
    node_ptr _left, _right;

public:
    zipper( int f ) : _focus( f ) {}

    bool shift( node_ptr &a, node_ptr &b )
    {
        auto new_b = std::move( b->next );
        auto new_a = std::move( b );
        new_a->next = std::move( a );

        std::swap( new_a->value, _focus );

        b = std::move( new_b );
        a = std::move( new_a );

        return true;
    }

    void push( node_ptr &p, int v )
    {
        p = std::make_unique< node >( v, std::move( p ) );
    }

    bool shift_left()
    {
        return _left  ? shift( _right, _left ) : false;
    }

    bool shift_right()
    {
        return _right ? shift( _left, _right ) : false;
    }

    void insert_left( int v )  { push( _left,  v ); }
    void insert_right( int v ) { push( _right, v ); }

    int &focus()       { return _focus; }
    int  focus() const { return _focus; }
};

