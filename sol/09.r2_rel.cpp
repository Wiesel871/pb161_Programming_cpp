struct relation
{
    std::map< int, std::set< int > > rel;
    std::function< bool( int, int ) > filter;

    void set_filter( auto f ) { filter = f; }
    void unset_filter() { filter = {}; }

    bool test( int a, int b ) const
    {
         return ( !filter || filter( a, b ) ) &&
                rel.contains( a ) &&
                rel.at( a ).contains( b );
    }

    auto get( int a ) const
    {
        std::set< int > out;

        if ( rel.contains( a ) )
            for ( int b : rel.at( a ) )
                if ( !filter || filter( a, b ) )
                    out.insert( b );

        return out;
    }

    void add( int a, int b )
    {
        rel[ a ].insert( b );
        rel[ b ].insert( a );
    }
};
