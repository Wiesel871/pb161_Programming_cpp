#include <cassert>

/* In this exercise, you will create a simple class: it will encapsulate some
 * state (account balance) and provide a simple, safe interface around that
 * state. The class should have the following interface:
 *
 *  • the constructor takes 2 integer arguments: the initial balance and the
 *    maximum overdraft
 *  • a ‹withdraw› method which returns a boolean: it performs the action and
 *    returns ‹true› iff there was sufficient balance to do the withdrawal
 *  • a ‹deposit› method which adds funds to the account
 *  • a ‹balance› method which returns the current balance (may be negative)
 *    and that can be called on ‹const› instances of ‹account› */

class account;

int main()
{
    /* it is okay to look at the balance on constant accounts */
    const account c( 300, 0 );
    assert( c.balance() == 300 );

    account a( 200, 100 );

    /* check that withdrawal works okay */
    assert( a.balance() == 200 );
    assert( a.withdraw( 100 ) );
    assert( a.balance() == 100 );
    assert( a.withdraw( 100 ) );
    assert( a.balance() == 0 );
    assert( a.withdraw( 100 ) );
    assert( a.balance() == -100 );

    /* withdrawal must fail if there are no funds */
    assert( !a.withdraw( 1 ) );
    assert( a.balance() == -100 );

    /* we can still deposit and then withdraw the money again */
    a.deposit( 1 );
    assert( a.balance() == -99 );
    assert( a.withdraw( 1 ) );
    assert( a.balance() == -100 );

    /* deposit a bit more */
    a.deposit( 100 );
    assert( a.balance() == 0 );

    /* bigger withdrawals must fail too */
    assert( !a.withdraw( 200 ) );
    assert( a.balance() == 0 );
    assert( !a.withdraw( 300 ) );
    assert( a.balance() == 0 );

    /* even if crossing the 0 boundary */
    a.deposit( 100 );
    assert( a.balance() == 100 );
    assert( !a.withdraw( 300 ) );
    assert( a.balance() == 100 );

    return 0;
}
