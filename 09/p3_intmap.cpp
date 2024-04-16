#include <cassert>
#include <cstddef>
#include <variant>
#include <vector>
#include <deque>

/* Navrhněte typ, který bude reprezentovat operaci nad polem celých
 * čísel. Vyhodnocení sestavené operace se provede metodou:
 *
 *  • ‹eval( v )› – aplikuje operaci na vektor celých čísel ‹v›
 *    (přepsáním vstupního vektoru),
 *
 * Celkový efekt operace bude lze postupně zadat připojováním
 * elementárních operací „na konec“ stávající operace ‹op› (‹n›
 * představuje celé číslo, ‹v› představuje vektor celých čísel):
 *
 *  • ‹op.add( n )› – přičte ke všem prvkům vstupu hodnotu ‹n› (tzn.
 *    ‹out[ i ] = in[ i ] + n›),
 *  • ‹op.add( v )› – cyklicky přičte hodnoty z ‹v› ke vstupnímu
 *    vektoru (tzn. ‹out[ i ] = in[ i ] + v[ i ]›; padne-li ‹i› mimo
 *    rozsah vektoru ‹v›, pokračuje se opět prvním prvkem ‹v›,
 *    atd. – můžete předpokládat, že ‹v› není prázdné),
 *  • ‹op.rotate( n )› – přesune prvek na indexu ‹i› na index ‹i + n›
 *    (tzn. ‹out[ i + n ] = in[ i ]›; je-li ‹i + n› mimo meze,
 *    použije se vhodný index v mezích tak, aby operace realizovala
 *    rotaci),
 *  • ‹op.pop()› – zapomene posledně vloženou elementární operaci.
 */

struct rotate_op {
    int n;

    rotate_op(int n) : n{n} {}
};

using operation = std::variant<int, std::vector<int>, rotate_op>;

struct intmap {
    std::deque<operation> q;

    void add(int x) {
        q.emplace_back(x);
    }

    void add(const std::vector<int> &v) {
        q.emplace_back(v);
    }

    void rotate(int n) {
        q.emplace_back(rotate_op{n});
    }

    void eval(std::vector<int> &v) {
        for (auto &op: q) {
            if (int *p = std::get_if<int>(&op); p) {
                for (int &x: v)
                    x += *p; 
            } else if (std::vector<int> *p = std::get_if<std::vector<int>>(&op); p) {
                for (std::size_t i = 0; i < v.size(); ++i)
                    v[i] += (*p)[i % p->size()];
            } else {
                int n = std::get<rotate_op>(op).n;
                n %= v.size();
                std::vector<int> aux(v.size(), 0);
                for (std::size_t i = 0; i < v.size(); ++i)
                    aux[(i + n) % v.size()] = v[i];
                v = std::move(aux);
            }
        }
    }

    void pop() {
        if (q.empty())
            return;
        q.pop_back();
    }
};

int main()
{
    intmap op{};
    std::vector< int > numbers( 6, 0 );

    op.add( { 1, 2 } );
    op.rotate( 1 );
    op.add( { 1, 2 } );
    op.eval( numbers );

    assert(( numbers == std::vector{ 3, 3, 3, 3, 3, 3 } ));

    op.pop();
    op.add( 3 );
    op.eval( numbers );

    assert(( numbers == std::vector{ 8, 7, 8, 7, 8, 7 } ));

    op.add( { 10, 20, 15, 4, 7, 6, 9, 33, 14 } );
    op.rotate( 4 );
    op.eval( numbers );

    assert(( numbers == std::vector{ 27, 16, 19, 18, 22, 32 } ));
    return 0;
}
