/* The base class. We keep track of the inputs using raw pointers,
 * since we do not own them. We use a ‹protected virtual› method to
 * implement the ‘business logic’ that changes from class to class,
 * while the outside interface is defined entirely using standard
 * (non-virtual) methods. */

class component
{
    component *left = nullptr,
              *right = nullptr;

protected:
    virtual bool eval( bool, bool ) = 0;

public:
    void connect( int n, component &c )
    {
        ( n ? right : left ) = &c;
    }

    bool read()
    {
        return eval( left ? left->read() : false,
                     right ? right->read() : false );
    }

    virtual ~component() = default;
};

/* The NAND gate and the ‹source› component are trivial enough. */

class nand : public component
{
    bool eval( bool x, bool y ) override { return !( x && y ); }
};

class source : public component
{
    bool eval( bool, bool ) override { return true; }
};

/* The ‹delay› component provides one bit of memory. Reading the
 * component will cause the value to be updated (‹read› always calls
 * ‹eval› internally). This class is also the reason why ‹eval›
 * cannot be marked ‹const›. */

class delay : public component
{
    bool _value = false;
    bool eval( bool x, bool ) override
    {
        bool rv = _value;
        _value = x;
        return rv;
    }
};

