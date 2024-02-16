struct node
{
    using ptr = std::shared_ptr< node >;
    enum op_t { cnst, var, add, mul, exp } op;
    int num = 0;
    ptr l, r;
};

class expr
{
public:
    node::ptr ptr;
    expr() : ptr( std::make_shared< node >() ) {}
    expr( int c ) : expr() { ptr->num = c; ptr->op = node::cnst; }
    expr( node::op_t o, node::ptr l = nullptr,
          node::ptr r = nullptr )
        : expr()
    {
        ptr->op = o;
        ptr->l = l;
        ptr->r = r;
    }
    expr( node::ptr p ) :ptr( p ) {}

    friend expr expnat( expr e )
    {
        return { node::exp, e.ptr };
    }

    friend expr operator+( expr a, expr b )
    {
        return { node::add, a.ptr, b.ptr };
    }

    friend expr operator*( expr a, expr b )
    {
        return { node::mul, a.ptr, b.ptr };
    }
};

const expr x{ node::var };

double eval( expr e, double v )
{
    switch ( e.ptr->op )
    {
        case node::cnst: return e.ptr->num;
        case node::var: return v;
        case node::add: return eval( e.ptr->l, v ) + eval( e.ptr->r, v );
        case node::mul: return eval( e.ptr->l, v ) * eval( e.ptr->r, v );
        case node::exp: return std::exp( eval( e.ptr->l, v ) );
    }
    abort();
}

expr diff( expr e )
{
    switch ( e.ptr->op )
    {
        case node::cnst: return { 0 };
        case node::var:  return { 1 };
        case node::add:
            return diff( e.ptr->l ) + diff( e.ptr->r );
        case node::mul:
            return diff( e.ptr->l ) * e.ptr->r +
                   diff( e.ptr->r ) * e.ptr->l;
        case node::exp:
            return e * diff( e.ptr->l );
    }
    abort();
}
