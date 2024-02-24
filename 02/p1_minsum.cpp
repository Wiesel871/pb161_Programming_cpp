#include <iostream>
#include <vector>
#include <cassert>

/* Na vstupu dostanete posloupnost celočíselných hodnot (jako
 * instanci kontejneru ‹std::vector›). Vaším úkolem je rozdělit je
 * na kratší posloupnosti tak, že každá posloupnost je nejkratší
 * možná, ale zároveň je její součet alespoň ‹sum›. Výjimku tvoří
 * poslední posloupnost, pro kterou nemusí nutně existovat potřebné
 * prvky.
 *
 * Pořadí prvků musí být zachováno, tzn. zřetězením všech
 * posloupností na výstupu musí vzniknout původní posloupnost
 * ‹numbers›. */
auto minsum( const std::vector< int > &numbers, int sum ) {
    auto res = std::vector<std::vector<int>>{{}};
    int cur_sum = 0;
    for (int n : numbers) {
        cur_sum += n;
        res.back().push_back(n);
        if (cur_sum >= sum) {
            cur_sum = 0;
            auto aux = std::vector<int>{};
            res.push_back(aux);
        }
    }
    if (res.back().empty())
        res.pop_back();
    return res;
}

int main()
{
    assert(( minsum( std::vector{ 1, 2 }, 1 ) ==
               std::vector{ std::vector{ 1 }, { 2 } } ));
    assert(( minsum( std::vector{ 1, 2, 3, 4 }, 3 ) ==
             std::vector{ std::vector{ 1, 2 }, { 3 }, { 4 } } ));

    return 0;
}
