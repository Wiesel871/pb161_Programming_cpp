struct qf;

qf make_qf( int a_nom, int a_den, int b_nom, int b_den );
qf make_qf( rat a, rat b );

struct qf
{
    rat u, v;

    friend bool operator==( const qf &a, const qf &b ) = default;

    friend qf operator+( const qf &a, const qf &b )
    {
        return make_qf( a.u + b.u, a.v + b.v );
    }

    friend qf operator*( const qf &a, const qf &b )
    {
        auto two = make_rat( 2, 1 );
        return make_qf( a.u * b.u + two * a.v * b.v,
                        a.u * b.v + a.v * b.u );
    }
};

qf make_qf( int a_nom, int a_den, int b_nom, int b_den )
{
    return make_qf( make_rat( a_nom, a_den ),
                    make_rat( b_nom, b_den ) );
}

qf make_qf( rat u, rat v )
{
    return qf{ .u = u, .v = v };
}
