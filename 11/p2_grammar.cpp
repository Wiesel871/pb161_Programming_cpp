#include <cassert>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <iostream>

/* Regulární gramatika má pravidla tvaru ⟦A → xB⟧ nebo ⟦A → x⟧ kde
 * ⟦A⟧, ⟦B⟧ jsou neterminály a ⟦x⟧ je terminál. */

/* Navrhněte typ ‹grammar›, kterého hodnoty lze implicitně sestrojit
 * a který má tyto 2 metody:
 *
 *  • ‹add_rule›, které lze předat 2 nebo 3 parametry:
 *    a. jeden znak, který reprezentuje levou stranu (velké písmeno
 *       anglické abecedy) a
 *    b. jeden terminál (malé písmeno), nebo jeden terminál a jeden
 *       neterminál,
 *  • ‹generate›, která přijme 2 parametry: startovní neterminál a
 *    kladné celé číslo, které reprezentuje maximální délku slova;
 *    výsledkem bude seznam (‹std::vector›) všech řetězců (slov),
 *    které lze takto vygenerovat, a to v lexikografickém pořadí. */

class grammar {
    std::map<char, std::vector<std::pair<char, char>>> rules;
    
    public:
    void add_rule(char nonterm, char term, char opterm = '\0') {
        auto it = rules.find(nonterm);
        if (it != rules.end()) {
            it->second.emplace_back(std::make_pair(term, opterm));
        } else {
            std::vector<std::pair<char, char>> aux;
            aux.emplace_back(std::make_pair(term, opterm));
            rules[nonterm] = std::move(aux);
        }
    }

    std::vector<std::string> generate(char nonterm, std::size_t depth) const {
        if (depth == 0 || !rules.contains(nonterm))
            return {};
        std::vector<std::string> res = {};
        for (const auto &[term, opterm]: rules.at(nonterm)) {
            res.emplace_back("");
            res.back().push_back(term);
            if (opterm) {
                auto sub = res.back();
                res.pop_back();
                for (const auto &s: generate(opterm, depth - 1)) {
                    res.push_back(sub + s);
                }
            } 
        }
        return res;
    }
};

int main()
{
    auto w = []( auto... words ) /* helper function */
    {
        return std::vector< std::string >{ words... };
    };

    grammar g;
    const grammar &cg = g;

    assert( cg.generate( 'A', 10 ) == w() );
    g.add_rule( 'A', 'a' );
    assert( cg.generate( 'A', 10 ) == w( "a" ) );

    g.add_rule( 'A', 'b', 'A' );
    assert( cg.generate( 'A', 3 ) == w( "a", "ba", "bba" ) );

    g.add_rule( 'A', 'x', 'X' );
    g.add_rule( 'X', 'x' );
    assert( cg.generate( 'A', 3 ) == w( "a", "ba", "bba", "bxx", "xx" ) );
    g.add_rule( 'X', 'x', 'X' );
    assert( cg.generate( 'X', 3 ) == w( "x", "xx", "xxx" ) );

    return 0;
}
