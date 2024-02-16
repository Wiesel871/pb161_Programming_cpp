#include <string_view>
#include <map>
#include <vector>
#include <cassert>
#include <brick-trace>

/* V této ukázce budeme implementovat jednoduché srovnání řetězce se
 * vzorkem, který má dva typy zástupných znaků:
 *
 *  • ‹*› nahrazuje libovolnou posloupnost znaků (i prázdnou),
 *  • ‹%› funguje stejně, ale místo nejdelší možné posloupnosti
 *    vybere nejkratší možnou (rozdíl se projeví pouze v přiřazení
 *    podřetězců jednotlivým zástupným znakům; viz také ‹main›).
 *
 * Speciální znaky lze „vypnout“ tím, že jim předepíšeme znak ‹\›.
 *
 * Krom samotné informace, zda zadaný řetězec vzorku vyhovuje,
 * budeme také požadovat řetězce, které ve zpracovaném textu
 * odpovídaly jednotlivým zástupným znakům (např. proto, abychom je
 * mohli něčím nahradit) – těmto budeme říkat „zachycené“ (angl.
 * captured). */

/* Samotné hledání vzorku implementujeme rekurzivně. Aktuální
 * „pohled“ do vzorku i do řetězce budeme reprezentovat typem
 * ‹std::string_view›, stejně tak zachycené podřetězce.
 *
 * Parametr ‹pat› reprezentuje vzorek, který musíme v řetězci ‹str›
 * nalézt. Rekurzivní volání odstraňují znaky ze začátku ‹pat›
 * a/nebo ‹str› tak, aby tyto obsahovaly dosud nezpracované sufixy.
 *
 * Parametry ‹c_idx› a ‹c_len› popisují právě řešený zástupný znak –
 * ‹c_idx› je jeho pořadové číslo (a tedy index v parametru
 * ‹capture›, kterému odpovídá) a ‹c_len› je délka prozatím
 * zachyceného podřetězce (je-li ‹c_len› nula, žádný zástupný znak
 * aktivní není.) */

bool glob_match( std::string_view pat,
                 std::string_view str,
                 std::vector< std::string_view > &capture,
                 int c_idx, int c_len )
{
    /* Nejprve vyřešíme triviální případy: prázdný řetězec vyhovuje
     * prázdnému vzorku, nebo vzorku který obsahuje jediný zástupný
     * znak, naopak je-li jedna strana neprázdná a druhá prázdná,
     * shoda je vyloučena. */

    if ( str.empty() && ( pat == "" || pat == "*" || pat == "%" ) )
        return true;
    if ( pat.empty() || str.empty() )
        return false;

    /* Nemáme-li rozpracovaný žádný zástupný znak, poznačíme si
     * možný začátek zachyceného řetězce. Není-li následující znak
     * zástupný, poznačený začátek se při zpracování dalšího znaku
     * vzorku posune. Nemůžeme se zde rozhodovat podle prvního znaku
     * vzorku, protože ten již může být rozpracovaný. */

    if ( !c_len )
        capture[ c_idx ] = str;

    /* Zpracování zástupného znaku má dvě možná pokračování:
     * zachycení můžeme ukončit a pokračovat ve srovnání vzorku
     * dalším znakem. V takovém případě se posuneme se na další
     * index ‹c_idx› a vynulujeme ‹c_len›. Posun na další znak
     * vzorku realizuje metoda ‹substr› typu ‹std::string_view› –
     * první parametr je index, od kterého má nový pohled začínat,
     * druhý (volitelný) určuje délku nového pohledu (implicitně je
     * maximální možná, tzn. do konce „rodičovského“ pohledu).
     *
     * Vede-li navíc ukončení záchytu k celkovému úspěchu, uložíme
     * výsledný zachycený řetězec na příslušný index seznamu
     * ‹capture›. */

    auto end_capture = [&]
    {
        auto p_suf = pat.substr( 1 );
        bool m = glob_match( p_suf, str, capture, c_idx + 1, 0 );
        if ( m )
            capture[ c_idx ] = capture[ c_idx ].substr( 0, c_len );
        return m;
    };

    /* Druhou možností je rozpracovaný zachycený řetězec prodloužit
     * o jeden znak a pokračovat tak v zpracování aktivního
     * zástupného znaku. Vzorek tedy zachováme (první znak je stále
     * zástupný) a ve zpracovaném řetězci se o jeden znak posuneme
     * (opět metodou ‹substr›).  */

    auto extend_capture = [&]
    {
        auto s_suf = str.substr( 1 );
        return glob_match( pat, s_suf, capture, c_idx, c_len + 1 );
    };

    /* Začíná-li vzorek speciálním znakem (zástupným nebo ‹\›),
     * zpracujeme jej. Nejkratší shodu se pokusíme přednostně
     * ukončit a prodloužíme ji pouze v případě, kdy toto rozhodnutí
     * nevede k nalezení shody. Naopak hladovou (nejdelší) shodu se
     * prioritně pokusíme prodloužit, a ukončíme ji pouze v situaci,
     * kdy by prodloužení zabránilo nalezení shody. */

    switch ( pat[ 0 ] )
    {
        case '%': return end_capture() || extend_capture();
        case '*': return extend_capture() || end_capture();
        case '\\': pat.remove_prefix( 1 ); break;
        default: ;
    }

    /* Je-li vzorek nebo řetězec vyčerpán, nebo vzorek začíná
     * obyčejným znakem, který se neshoduje s odpovídajícím znakem
     * řetězce, shodu jsme nenašli. */

    if ( pat.empty() || str.empty() || pat[ 0 ] != str[ 0 ] )
        return false;

    /* V opačném případě je možné dosud nalezenou shodu prodloužit
     * o jeden znak a pokračovat ve zpracování sufixů. */

    pat.remove_prefix( 1 );
    str.remove_prefix( 1 );

    return glob_match( pat, str, capture, c_idx, 0 );
}

/* Před samotným rekurzivním zpracováním si nachystáme seznam
 * zachycených řetězců (abychom nemuseli při zpracování jednotlivých
 * znaků neustále kontrolovat, máme-li v ‹capture› dostatek místa).
 * Pro pohodlnější použití zachycené řetězce předáme volajícímu
 * jako součást návratové hodnoty. */

auto glob_match( std::string_view pat, std::string_view str )
{
    std::vector< std::string_view > capture;
    char prev = 0;

    for ( char c : pat )
    {
        if ( ( c == '*' || c == '%' ) && prev != '\\' )
            capture.emplace_back();
        prev = c;
    }

    bool match = glob_match( pat, str, capture, 0, 0 );

    if ( !match )
        capture.clear();

    return std::tuple{ match, capture };
}

int main() /* demo */
{
    using sv_vec = std::vector< std::string_view >;

    bool m;
    sv_vec capture;

    std::tie( m, capture ) = glob_match( "%.*", "x.y.z" );
    assert(( m && capture == sv_vec{ "x", "y.z" } ));

    std::tie( m, capture ) = glob_match( "*.%", "x.y.z" );
    assert(( m && capture == sv_vec{ "x.y", "z" } ));

    std::tie( m, capture ) = glob_match( "%.%", "x.y.z" );
    assert(( m && capture == sv_vec{ "x", "y.z" } ));

    std::tie( m, capture ) = glob_match( "\\%.%", "%.y.z" );
    assert(( m && capture == sv_vec{ "y.z" } ));

    std::tie( m, capture ) = glob_match( "%.%", "x:y:z" );
    assert(( !m && capture == sv_vec{} ));
}
