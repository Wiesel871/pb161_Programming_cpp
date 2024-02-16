#include <vector>
#include <cassert>

/* Consider a single-player game that takes place on a 1D playing
 * field like this:
 *
 *        ╭───────────────╮
 *        │               ▼
 *  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┐
 *  │ 2 │ 4 │ … │ … │   │ 2 │   │ … │ 0 │
 *  └───┴───┴───┴───┴───┴───┴───┴───┴───┘
 *    │       ▲   ▲      │ │      ▲
 *    ╰───────╯   ╰──────╯ ╰──────╯
 *
 * The player starts at the leftmost cell and in each round can
 * decide whether to jump left or right. The playing field is given
 * by the input vector ‹jumps›. The size of the field is
 * ‹jumps.size() + 1› (the rightmost cell is always 0). The
 * objective is to visit each cell exactly once. */

bool solve( std::vector< int > jumps );

int main()
{
    assert(  solve( { 1, 1, 1 } ) );
    assert(  solve( { 1, 1, 1, 1 } ) );
    assert( !solve( { 2, 2, 2, 2 } ) );
    assert( !solve( { 2, 2 } ) );
    assert(  solve( { 2, 2, 1 } ) );
    assert( !solve( { 3, 2, 1 } ) );
    assert(  solve( { 3, 1, 2, 2 } ) );
    assert(  solve( { 3, 3, 1, 1 } ) );
    assert( !solve( { 2 } ) );
    assert( !solve( { 2, 3, 1 } ) );
    assert( !solve( { 2, 1, 3 } ) );

    return 0;
}
