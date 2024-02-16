struct iota_iterator
{
    using iterator = iota_iterator;
    int _val;
    bool operator==( iterator o ) const { return _val == o._val; };
    bool operator!=( iterator o ) const { return _val != o._val; };
    iota_iterator &operator++() { ++ _val; return *this; }
    int operator*() const { return _val; }
};

struct iota
{
    int _start, _end;

    iota_iterator begin() const { return { _start }; }
    iota_iterator end()   const { return { _end }; }
    iota( int s, int e ) : _start( s ), _end( e ) {}
};
