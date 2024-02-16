struct validate
{
    std::string_view todo;

    std::string_view peek()
    {
        while ( !todo.empty() && std::isspace( todo[ 0 ] ) )
            todo.remove_prefix( 1 );

        unsigned i = 0;

        while ( i < todo.size() && !std::isspace( todo[ i ] ) )
            ++ i;

        return todo.substr( 0, i );
    }

    std::string_view fetch()
    {
        auto rv = peek();
        todo.remove_prefix( rv.size() );
        return rv;
    }

    bool is_ident( std::string_view s )
    {
        for ( auto c : s )
            if ( !std::isalpha( c ) )
                return false;
        return true;
    }

    bool ident()
    {
        return is_ident( fetch() );
    }

    bool atom()
    {
        auto next = fetch();

        if ( next == "0" || next == "1" )
            return true;
        else
            return is_ident( next );
    }

    bool expr()
    {
        if ( !atom() ) return false;

        if ( peek() == "+" || peek() == "-" )
        {
            fetch();

            if ( !atom() )
                return false;
        }

        return true;
    }

    bool stmt()
    {
        auto next = fetch();

        if ( next == "if" )
        {
            if ( !expr() ) return false;
            if ( fetch() != "then" ) return false;
            if ( !stmt() ) return false;
        }
        else if ( next == "while" )
        {
            if ( !expr() ) return false;
            if ( fetch() != "do" ) return false;
            if ( !stmt() ) return false;
        }
        else if ( next == "set" )
        {
            if ( !ident() ) return false;
            if ( fetch() != ":=" ) return false;
            if ( !expr() ) return false;
        }
        else if ( next == "begin" )
        {
            if ( !stmt() ) return false;
            while ( peek() != "end" )
                if ( !stmt() ) return false;
            if ( fetch() != "end" )
                assert( false );
        }
        else if ( next == "skip" )
            return true;
        else
            return false;

        return true;
    }

    bool top()
    {
        return stmt() && todo.empty();
    }
};

bool stmt_valid( std::string_view s )
{
    return validate{ s }.top();
}
