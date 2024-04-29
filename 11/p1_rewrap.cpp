#include <cstddef>
#include <cstdio>
#include <iterator>
#include <string>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

/* V tomto příkladu budeme pracovat s textem. Procedura ‹rewrap›
 * dostane odkaz (referenci) na řetězec, který obsahuje text složený
 * ze slov oddělených mezerami (U+0020) nebo znaky nového řádku
 * (U+000A). Dva nebo více znaků konce řádku těsně za sebou chápeme
 * jako oddělovač odstavců.
 *
 * Dále dostane celočíselný počet sloupců (znaků) ‹cols›, který
 * určují, jak dlouhý může být jeden řádek. Procedura pak
 * přeformátuje vstupní řetězec tak, aby:
 *
 *  1. bylo zachováno rozdělení na odstavce,
 *  2. jednotlivý řádek textu nepřesáhl ‹cols›,
 *  3. zachová celistvost slov (tzn. smí měnit pouze mezery a znaky
 *     nového řádku),
 *  4. každý řádek byl nejdelší možný.
 *
 * Můžete předpokládat, že žádné slovo není delší než ‹cols› a že
 * každá mezera sousedí po obou stranách se slovem. */
enum class sep {
    word = 0,
    new_line = 1,
    space = 2,
    paragraph = 3,
};
using word = std::u32string;
using paragraph = std::pair<std::vector<word>, std::size_t>;
using text = std::vector<paragraph>;

void print(word &w) {
    printf("word: \"");
    for (auto &c: w) {
        printf("%c", static_cast<char>(c));
    }
    printf("\"\n");
}

void print(paragraph &p) {
    printf("paragraph: %lu\n", p.second);
    for (auto &w: p.first) {
        print(w);
    }
}

void print(text &t) {
    printf("text:\n");
    for (auto &p: t) {
        print(p);
    }
}

void rewrap( std::u32string &str, int cols ) {
    text aux = {{{U""}, 0}};
    sep last_sep = sep::word;
    std::size_t p_size = 0;
    (void) cols;
    for (auto &c: str) {
        if (c == '\n') {
            if (last_sep == sep::new_line || last_sep == sep::paragraph) {
                ++p_size;
                last_sep = sep::paragraph;
            } else {
                last_sep = sep::new_line;
            }
        } else if (c == ' ') {
            last_sep = sep::space;
        } else {
            if (last_sep == sep::paragraph) {
                aux.push_back({{U""}, p_size});
            } else if (last_sep == sep::new_line || last_sep == sep::space) {
                aux.back().first.emplace_back(U"");
            }
            aux.back().first.back().push_back(c);
            last_sep = sep::word;
        }
    }
    std::size_t line_size = 0;
    str = U"";
    for (auto &p: aux) {
        for (std::size_t i = 0; i < p.second; ++i)
            str.push_back('\n');
        line_size = 0;
        for (auto &w: p.first) {
            if (w.data() != p.first.front().data()) {
                if (w.size() + 1 + line_size <= static_cast<std::size_t>(cols)) {
                    str.push_back(' ');
                    ++line_size;
                } else {
                    str.push_back('\n');
                    line_size = 0;
                }
            }
            std::copy(w.begin(), w.end(), std::back_inserter(str));
            line_size += w.size();
        }
        str.push_back('\n');
    }
}

int main()
{
    std::u32string s = U"lorem ipsum dolor sit amet\n";

    rewrap( s, 5 );
    assert( s == U"lorem\nipsum\ndolor\nsit\namet\n" );
    rewrap( s, 6 );
    assert( s == U"lorem\nipsum\ndolor\nsit\namet\n" );
    rewrap( s, 20 );
    assert( s == U"lorem ipsum dolor\nsit amet\n" );
    rewrap( s, 30 );
    assert( s == U"lorem ipsum dolor sit amet\n" );

    s = U"no\n\nway\n";
    rewrap( s, 100 );
    assert( s == U"no\n\nway\n" );

    return 0;
}
