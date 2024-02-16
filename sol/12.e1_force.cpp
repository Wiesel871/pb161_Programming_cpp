class force
{
    double x = 0, y = 0, z = 0;
public:
    force( double x, double y, double z )
        : x( x ), y( y ), z( z )
    {}

    force() = default;

    bool operator==( const force &f ) const
    {
        return std::fabs( f.x - x ) < 1e-10 &&
               std::fabs( f.y - y ) < 1e-10 &&
               std::fabs( f.z - z ) < 1e-10;
    }

    friend std::ostream &operator<<( std::ostream &o,
                                     const force &f )
    {
        return o << "[" << f.x << " " << f.y << " " << f.z << "]";
    }

    friend std::istream &operator>>( std::istream &i, force &f )
    {
        char ch;

        if ( !( i >> ch ) || ch != '[' )
            i.setstate( i.failbit );

        i >> f.x >> f.y >> f.z;

        if ( !( i >> ch ) || ch != ']' )
            i.setstate( i.failbit );

        return i;
    }
};
