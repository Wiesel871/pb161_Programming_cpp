#include <cassert>

/* In this example, we will demonstrate the syntax and most basic
 * use of inheritance. Polymorphism will not enter the picture yet
 * (but we will get to that very soon: in the next example). We will
 * consider bank accounts (a favourite subject, surely). */

/* We will start with a simple, vanilla account that has a balance,
 * can withdraw and deposit money. We have seen this before. */

class account
{
    /* The first new piece of syntax is the ‹protected› keyword.
     * This is related to inheritance: unlike ‹private›, it lets
     * «subclasses» (or rather «subclass methods») access the
     * members declared in a ‹protected› section. We also notice
     * that the balance is signed, even though in this class, that
     * is not strictly necessary: we will need that in one of the
     * subclasses (yes, the system is «already» breaking down a
     * little). */

protected:
    int _balance;

public:

    /* We allow an account to be constructed with an initial
     * balance. We also allow it to be default-constructed,
     * initializing the balance to 0. */

    account( int initial = 0 )
        : _balance( initial )
    {}

    /* Standard stuff. */

    bool withdraw( int sum )
    {
        if ( _balance > sum )
        {
            _balance -= sum;
            return true;
        }

        return false;
    }

    void deposit( int sum ) { _balance += sum; }
    int balance() const { return _balance; }
};

/* With the base class in place, we can define a «derived» class.
 * The syntax for inheritance adds a colon, ‹:›, after the class
 * name and a list of classes to inherit from, with access type
 * qualifiers. We will always use ‹public› inheritance. Also, did
 * you know that naming things is hard? */

class account_with_overdraft : public account
{
    /* The derived class has, ostensibly, a single attribute.
     * However, all the attributes of all base classes are also
     * present automatically. That is, there already is an ‹int
     * _balance› attribute in this class, inherited from ‹account›.
     * We will use it below. */

protected:
    int _overdraft;

public:

    /* This is another new piece of syntax that we will need: a
     * constructor of a derived class must first call the
     * constructors of all base classes. Since this happens «before»
     * any attributes of the derived class are constructed, this
     * call comes «first» in the «initialization section». The
     * derived-class constructor is free to choose which
     * (overloaded) constructor of the base class to call. If the
     * call is omitted, the «default constructor» of the base class
     * will be called. */

    account_with_overdraft( int initial = 0, int overdraft = 0 )
        : account( initial ), _overdraft( overdraft )
    {}

    /* The methods defined in a base class are automatically
     * available in the derived class as well (same as attributes).
     * However, unlike attributes, we can replace inherited methods
     * with versions more suitable for the derived class. In this
     * case, we need to adjust the behaviour of ‹withdraw›. */

    bool withdraw( int sum )
    {
        if ( _balance + _overdraft > sum )
        {
            _balance -= sum;
            return true;
        }

        return false;
    }
};

/* Here is another example based on the same language features. */

class account_with_interest : public account
{
protected:
    int _rate; /* percent per annum */

public:

    account_with_interest( int initial = 0, int rate = 0 )
        : account( initial ), _rate( rate )
    {}

    /* In this case, all the inherited methods can be used directly.
     * However, we need to «add» a new method, to compute and
     * deposit the interest. Since naming things is hard, we will
     * call it ‹next_year›. The formula is also pretty lame. */

    void next_year()
    {
        _balance += ( _balance * _rate ) / 100;
    }
};

/* The way objects are used in this exercise is not super useful:
 * the goal was to demonstrate the syntax and basic properties of
 * inheritance. In modern practice, code re-use through inheritance
 * is frowned upon (except perhaps for mixins, which are however out
 * of scope for this subject). The main use-case for inheritance is
 * «subtype polymorphism», which we will explore in the next unit,
 * ‹shapes.cpp›. */

int main() /* demo */
{
    /* We first make a normal account and check that it behaves as
     * expected. Nothing much to see here. */

    account a( 100 );
    assert( a.balance() == 100 );
    assert( a.withdraw( 50 ) );
    assert( !a.withdraw( 100 ) );
    a.deposit( 10 );
    assert( a.balance() == 60 );

    /* Let's try the first derived variant, an account with
     * overdraft. We notice that it's possible to have a negative
     * balance now. */

    account_with_overdraft awo( 100, 100 );
    assert( awo.balance() == 100 );
    assert( awo.withdraw( 50 ) );
    assert( awo.withdraw( 100 ) );
    awo.deposit( 10 );
    assert( awo.balance() == -40 );

    /* And finally, let's try the other account variant, with
     * interest. */

    account_with_interest awi( 100, 20 );
    assert( awi.balance() == 100 );
    assert( awi.withdraw( 50 ) );
    assert( !awi.withdraw( 100 ) );
    awi.deposit( 10 );
    assert( awi.balance() == 60 );
    awi.next_year();
    assert( awi.balance() == 72 );
}
