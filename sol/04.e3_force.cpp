struct force
{
    double x, y, z; /* cartesian components of the force */

    force( double x, double y, double z )
        : x( x ), y( y ), z( z ) {}

    /* We only define multiplication by a scalar (‹double›) from
     * left, since we only need that here, but it would be equally
     * valid to flip the operand types (and define scalar
     * multiplication on the right). */

    friend force operator*( double s, force f )
    {
        return { s * f.x, s * f.y, s * f.z };
    }

    /* Bog-standard vector addition. */

    friend force operator+( force a, force b )
    {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    /* Fuzzy vector equality. Two vectors are equal when all their
     * components are equal. */

    friend bool operator==( force a, force b )
    {
        return std::fabs( a.x - b.x ) < 1e-10 &&
               std::fabs( a.y - b.y ) < 1e-10 &&
               std::fabs( a.z - b.z ) < 1e-10;
    }
};
