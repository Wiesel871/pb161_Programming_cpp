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

std::vector< std::u32string_view > words( std::u32string_view );

/* ¹ Skutečná segmentace textu je «velmi» složitá a prakticky jediná
 *   možnost je použít stávající knihovny, pro C++ např. ICU4C
 *   (balík knihoven, který má dohromady cca 100MiB a jen hlavičkové
 *   soubory mají cca 120 tisíc řádků). */

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
