using _pub_::counter;

struct counted
{
    counted() { ++ counter; }

    counted( const counted & ) { ++counter; }
    counted( counted && ) { ++counter; }

    counted &operator=( const counted & ) = default;
    counted &operator=( counted && ) = default;

    ~counted() { --counter; }
};
