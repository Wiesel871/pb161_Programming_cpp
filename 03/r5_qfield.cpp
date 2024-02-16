/* Uvažme těleso ⟦ℚ⟧ a číslo ⟦j⟧ takové, že ⟦j² = 2⟧ (tedy zejména
 * ⟦j ∉ ℚ⟧). Těleso ⟦ℚ⟧ můžeme rozšířit na tzv. (algebraické)
 * číselné těleso (v tomto případě konkrétně kvadratické těleso)
 * ⟦ℚ(j)⟧, kterého prvky jsou tvaru ⟦a + bj⟧, kde ⟦a, b ∈ ℚ⟧.
 *
 * Vaším úkolem je naprogramovat typ, který prvky tohoto tělesa
 * reprezentuje, a umožňuje je sčítat, násobit a rozhodovat jejich
 * rovnost.  Rozmyslete si vhodnou reprezentaci; uvažte zejména jak
 * bude vypadat výsledek násobení ⟦(a + bj)⋅(x + yj)⟧. */

struct qf;

/* Protože k dispozici máme pouze celá čísla, k zápisu jednoho prvku
 * ⟦ℚ(j)⟧ budeme potřebovat 4 (vystačili bychom si se třemi? pokud
 * ano, jaké to má výhody a nevýhody?). */

qf make_qf( int a_nom, int a_den, int b_nom, int b_den );

int main()
{
    qf a = make_qf( 1, 1, 0, 1 ),
       b = make_qf( 2, 1, 0, 1 ),
       c = make_qf( 1, 1, 1, 1 ),
       j = make_qf( 0, 1, 1, 1 );

    assert( a == a );
    assert( b == b );
    assert( a != b );
    assert( a * b == b );
    assert( a + b != b );
    assert( a + b != a );
    assert( a + a == b );

    assert( a + j == c );
    assert( a * j == j );
    assert( j * j == a + a );

    return 0;
}
