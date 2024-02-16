#include <vector>
#include <cassert>

/* V tomto cvičení naprogramujeme vyhodnocování infixových
 * aritmetických výrazů. Zároveň zabezpečíme, aby bylo lze sdílet
 * společné podvýrazy (tzn. uložit je jenom jednou a při dalším
 * výskytu je pouze odkázat). Proto budeme uzly ukládat ve
 * společném úložišti. */

const int op_mul = 1;
const int op_add = 2;
const int op_num = 3;

struct node
{
    /* Operace, kterou tento uzel reprezentuje (viz konstanty
     * definované výše). Pouze uzly typu ‹mul› a ‹add› mají
     * potomky. */

    int op;

    /* Položky ‹left› a ‹right› jsou indexy, přičemž hodnota -1
     * značí neplatný odkaz. Položka ‹is_root› je nastavena na
     * ‹true› právě tehdy, když tento uzel není potomkem žádného
     * jiného uzlu. */

    int left = -1, right = -1;
    bool is_root = true;

    /* Hodnota uzlu, je-li tento uzel typu ‹op_num›. */

    int value = 0;
};

using node_pool = std::vector< node >;

/* Dočasná reference na uzel, kterou lze použít při procházení
 * stromu, ale která je platná pouze tak dlouho, jako hodnota typu
 * ‹eval›, která ji vytvořila. Přidejte (konstantní) metody ‹left› a
 * ‹right›, kterých výsledkem je nová hodnota typu ‹node_ref›
 * popisující příslušný uzel, a dále metodu ‹compute›, která
 * vyhodnotí podstrom začínající v aktuálním uzlu. Konečně přidejte
 * metodu ‹update›, která upraví hodnotu v aktuálním uzlu. Metodu
 * ‹update› je dovoleno použít pouze na uzly typu ‹op_num›. */

struct node_ref;

/* Typ ‹eval› reprezentuje výraz jako celek. Umožňuje vytvářet nové
 * výrazy ze stávajících (pomocí metod ‹add›, ‹mul› a ‹num›) a
 * procházet strom výrazů (počínaje z kořenů, které lze získat
 * metodou ‹roots›). */

struct eval
{
    node_pool _pool;

    std::vector< node_ref > roots();

    node_ref add( node_ref, node_ref );
    node_ref mul( node_ref, node_ref );
    node_ref num( int );
};

int main()
{
    auto root = []( eval &e )
    {
        assert( e.roots().size() == 1 );
        return e.roots().front();
    };

    eval e;

    node_ref a = e.number( 7 );
    node_ref b = e.number( 8 );
    node_ref sum = e.add( a, b );
    node_ref mul = e.mul( sum, sum );

    assert( sum.compute() == 15 );
    assert( mul.compute() == 225 );

    eval f = e;

    assert( root( f ).compute() == 225 );
    assert( root( f ).left().compute() == 15 );

    node_ref r = f.add( root( f ), root( f ) );
    assert( r.compute() == 450 );
    assert( root( e ).compute() == 225 );

    e = f;
    assert( root( e ).compute() == 450 );

    return 0;
}
