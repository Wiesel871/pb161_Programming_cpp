struct validate
{
    std::string_view todo;

    char peek() { return todo.empty() ? 0 : todo[ 0 ]; }
    char fetch() { auto c = peek(); todo.remove_prefix( 1 ); return c; }

    void ws()
    {
         while ( std::isspace( peek() ) )
             fetch();
    }

    bool maybe_ident()
    {
        int i = 0;
        while ( std::isalpha( peek() ) )
            ++i, fetch();
        return i;
    }

    bool maybe_num()
    {
        int i = 0;
        while ( std::isdigit( peek() ) )
            ++ i, fetch();
        return i;
    }

    bool factor()
    {
        ws();

        if ( peek() == '(' )
        {
            fetch();
            if ( !expr() ) return false;
            if ( ws(), fetch() != ')' ) return false;
            return true;
        }
        else
            return maybe_num() || maybe_ident();
    }

    bool list( auto f, char c )
    {
        ws();
        if ( !f() ) return false;
        while ( ws(), peek() == c )
            if ( fetch(), !f() )
                return false;
        return true;
    }

    bool expr()
    {
        return list( [&]{ return term(); }, '+' );
    }

    bool term()
    {
        return list( [&]{ return factor(); }, '*' );
    }

    bool top()
    {
        return expr() && todo.empty();
    }
};

bool expr_valid( std::string_view s )
{
    return validate{ s }.top();
}
