#include <cassert>

/* Navrhněte typ ‹sched_queue›, který bude udržovat prioritní frontu
 * hodnot typu ‹task›, uspořádanou podle složky ‹priority›. Mají-li
 * dva prvky stejnou prioritu, přednost má ten s nižším ‹id›. Typ
 * ‹task› nemodifikujte. */

struct task
{
    int priority, static_priority, id;
};

/* Typ ‹sched_queue› nechť poskytuje tyto operace:
 *
 *  • ‹add› vloží prvek do fronty,
 *  • ‹peek› vrátí konstantní odkaz na prvek s nejvyšší prioritou,
 *  • ‹demote› podobně, ale vrátí nekonstantní odkaz a zároveň prvku
 *    sníží prioritu o jedna,
 *  • ‹reset› nastaví prioritu všech prvků na jejich hodnotu
 *    ‹static_priority›.
 *
 * Všechny operace s výjimkou ‹peek› mohou zneplatnit reference
 * vrácené některým předchozím voláním ‹peek› nebo ‹demote›. */

struct sched_queue;

int main()
{
    sched_queue q;
    const auto &const_q = q;

    q.add( { 3, 3, 1 } );
    q.add( { 3, 3, 2 } );
    q.add( { 2, 2, 3 } );

    task &t_1 = q.demote();
    assert( t_1.id == 1 );
    assert( t_1.priority == 2 );

    task &t_2 = q.demote();
    assert( t_2.id == 2 );
    assert( t_2.priority == 2 );

    assert( const_q.peek().id == 1 );
    q.demote();
    q.demote();
    assert( const_q.peek().id == 3 );

    q.reset();
    assert( q.peek().id == 1 );
    q.demote();
    assert( q.peek().id == 2 );

    return 0;
}
