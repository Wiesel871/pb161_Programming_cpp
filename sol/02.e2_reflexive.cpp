using _pub_::relation;

relation reflexive( const relation &r )
{
    relation out = r;

    for ( auto [ x, y ] : r )
    {
        out.emplace( x, x );
        out.emplace( y, y );
    }

    return out;
}
