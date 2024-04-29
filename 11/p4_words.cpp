#include <cstdint>
#include <cstdio>
#include <string_view>
#include <vector>
#include <string>
#include <cassert>

/* Napište čistou funkci, která spočítá naivní rozdělení textu na
 * jednotlivá slova.¹ Budeme uvažovat pouze bílé vs nebílé znaky
 * (kódové body), a za slova budeme považovat libovolnou neprázdnou
 * sekvenci nebílých znaků. Unicode obsahuje tyto bílé znaky
 * (označené vlastností ‹White_Space›):
 *
 * > ‹U+0009› – ‹U+000D›, ‹U+0020›, ‹U+0085›, ‹U+00A0›, ‹U+1680›,
 * > ‹U+2000› – ‹U+200A›, ‹U+2028›, ‹U+2029›, ‹U+202F›, ‹U+205F›,
 * > ‹U+3000›.
 *
 * Dále budeme považovat za bílé i znaky ‹U+200B›, ‹U+2060›, které
 * logicky (ale ne vizuálně) slova oddělují (tyto znaky vlastností
 * ‹White_Space› označeny nejsou).
 *
 * Vstupem funkce je pohled na text, výstupem funkce je seznam
 * (‹std::vector›) pohledů, které vymezují jednotlivá identifikovaná
 * slova. */
bool is_space(std::uint32_t c) {
    if (c >= U'\u0009' && c <= U'\u000d')
        return true;
    switch (c) {
        case U'\u0020':
        case U'\u0085':
        case U'\u00a0':
        case U'\u1680':
        case U'\u2028':
        case U'\u2029':
        case U'\u202f':
        case U'\u205f':
        case U'\u2060':
        case U'\u3000': return true;
    };
    return c >= U'\u2000' && c <= U'\u200b';
}

std::vector< std::u32string_view > words( std::u32string_view strv) {
    std::vector<std::u32string_view> res = {};
    auto beg = strv.begin();
    auto end = beg;
    while (end != strv.end()) {
        if (is_space(*end)) {
            if (beg != end) {
                res.emplace_back(beg, end);
                beg = end;
            } else {
                ++beg;
                ++end;
            }
        } else {
            ++end;
        }
    }
    if (beg != end)
        res.emplace_back(beg, end);
    return res;
}

/* ¹ Skutečná segmentace textu je «velmi» složitá a prakticky jediná
 *   možnost je použít stávající knihovny, pro C++ např. ICU4C
 *   (balík knihoven, který má dohromady cca 100MiB a jen hlavičkové
 *   soubory mají cca 120 tisíc řádků). */
void print(std::vector<std::u32string_view> l) {
    for (auto &w: l) {
        printf("word: \"");
        for (auto &c: w) {
            printf("%c", static_cast<char>(c));
        }
        printf("\"\n");
    }
}

int main()
{
    using uvec = std::vector< std::u32string_view >;

    assert(( words( U"lorem ipsum dolor" ) ==
             uvec{ U"lorem", U"ipsum", U"dolor" } ));
    assert(( words( U"hello, world" ) == uvec{ U"hello,", U"world" } ));
    assert(( words( U"hello  world" ) == uvec{ U"hello", U"world" } ));
    assert(( words( U"hello\nworld\n" ) == uvec{ U"hello", U"world" } ));
    assert(( words( U" hello \n world" ) == uvec{ U"hello", U"world" } ));
    assert(( words( U"hello world" ) == uvec{ U"hello", U"world" } ));
    assert(( words( U"hello world" ) == uvec{ U"hello", U"world" } ));
    assert(( words( U"lorem\tipsum\vdolor" ) ==
             uvec{ U"lorem", U"ipsum", U"dolor" } ));
    assert(( words( U"lorem ipsum​dolor" ) ==
             uvec{ U"lorem", U"ipsum", U"dolor" } ));

    return 0;
}
