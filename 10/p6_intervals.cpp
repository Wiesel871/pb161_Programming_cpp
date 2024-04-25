#include <cassert>
#include <tuple>
#include <vector>
#include <set>

/* Naprogramujte proceduru ‹intervals›, která z posloupnosti ⟦k⟧
 * dvojic (zleva uzavřených, zprava otevřených intervalů) vytvoří
 * vzestupně seřazenou posloupnost ⟦n⟧ prvků tak, že každá hodnota,
 * která spadá do některého vstupního intervalu, se ve výstupní
 * posloupnosti objeví právě jednou.
 *
 * Procedura ‹intervals› bude mít rozhraní podobné standardním
 * algoritmům:
 *
 *  • vstupem bude dvojice (rozsah) iterátorů, které zadávají
 *    sekvenci dvojic–intervalů (‹std::tuple›),
 *  • a výstupní iterátor, do kterého zapíše výslednou posloupnost.
 *
 * Můžete předpokládat, že prvky (a tedy i intervaly zadané jako
 * jejich dvojice) lze kopírovat a přiřazovat. Algoritmus by měl mít
 * složitost ⟦O(n + k⋅log(k))⟧. */

std::set<int> iota(int start, int end, std::set<int> &res) {
    while (start < end)
        res.insert(start++);
    return res;
}

void intervals(auto begin, auto end, auto insert) {
    std::set<int> res = {};
    std::vector<std::tuple<int, int>> used;
    while (begin != end) {
        iota(std::get<0>(*begin), std::get<1>(*begin), res);
        ++begin;
    }
    std::copy(res.begin(), res.end(), insert);
}

int main()
{
    using interval = std::tuple< int, int >;
    std::vector< interval > iv{ { 0, 3 }, { 7, 8 }, { 2, 3 }, { 1, 5 } };

    std::vector< int > vals;
    intervals( iv.cbegin(), iv.cend(), std::back_inserter( vals ) );
    assert(( vals == std::vector{ 0, 1, 2, 3, 4, 7 } ));

    iv.pop_back();
    intervals( iv.cbegin(), iv.cend(), std::back_inserter( vals ) );
    assert(( vals == std::vector{ 0, 1, 2, 3, 4, 7, 0, 1, 2, 7 } ));

    return 0;
}
