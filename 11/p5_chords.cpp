#include <cassert>
#include <string>

/* V tomto cvičení se budeme zabývat hudebními akordy. Nezapomeňte
 * kód dekomponovat do smysluplných celků. */

/* Tzv. „západní“ hudební tradice používá 12 tónů. Sousední tóny
 * jsou vzdálené jeden půltón (100 centů). Základní akordy jsou
 * vystavěny z malých (300 centů) a velkých (400 centů) tercií.
 * Budeme se zabývat pouze akordy v základním tvaru, tzn. základní
 * tón je zároveň basovým. K zápisu budeme používat německé
 * názvosloví:
 *
 *  • c, d, e, f, g, a, h jsou „základní“ tóny bez posuvek (♮),
 *  • cis, dis, eis = f, fis, gis, ais, his = c → s křížkem (♯),
 *  • ces = h, des, es, fes = e, ges, as, b → tóny s béčkem (♭).
 *
 * Základní noty (♮) jsou vzdálené 200 centů, s výjimkou dvojic e/f
 * a h/c, které jsou vzdálené pouze 100 centů. Béčko odečítá a
 * křížek přičítá k hodnotě noty 100 centů. Zjednodušená pravidla
 * pro používání názvů tónů při stavbě akordů:
 *
 *  • v tóninách C, G, D, A, E, H, Fis, Cis → používáme křížky,
 *  • tóniny F, B, Es, As, Des, Ges, Ces → používáme béčka,
 *  • béčka a křížky (v základních akordech) nemícháme,
 *  • dvojitá béčka a křížky neuvažujeme,
 *  • místo eis, his, ces, fes, použijeme f, c, h, e.
 *
 * Čistá kvinta je 700 centů, zatímco malá septima je 1000 centů.
 * Intervaly (vyjádřené v centech) skládáme sčítáním mod 1200,
 * přitom konvenčně chápeme tón c jako nulu.
 *
 * Je-li například základní tón g, tzn. 700 centů, přičtením čisté
 * kvinty dostaneme 1400 mod 1200 = 200 centů, neboli tón d. Mezi
 * tóny g a d je tedy interval čisté kvinty. */

/* Durový kvintakord stavíme od základního tónu tóniny přidáním
 * velké tercie a další malé tercie, například c → c e g nebo e → e
 * gis h. */

std::string major_5( std::string key );

/* Mollový kvintakord stavíme od sexty (900 centů) paralelní durové
 * tóniny přidáním malé tercie a další velké tercie, např.
 * c → a c e, nebo e → cis e gis. */

std::string minor_5( std::string key );

/* Dominantní septakord stavíme od kvinty durové tóniny, např.
 * v tónině C bude postaven na tónu g, a vznikne přidáním jedné
 * velké a dvou malých tercií (celkem 4 tóny), například tedy
 * f → c e g b. */

std::string dominant_7( std::string key );

int main()
{
    assert( major_5( "c" )  == "c e g" );
    assert( major_5( "a" )  == "a cis e" );
    assert( major_5( "f" )  == "f a c" );
    assert( major_5( "fis" )  == "fis ais cis" );
    assert( major_5( "e" )  == "e gis h" );

    assert( minor_5( "es" ) == "c es g" );
    assert( minor_5( "c" )  == "a c e" );
    assert( minor_5( "d" )  == "h d fis" );
    assert( minor_5( "e" )  == "cis e gis" );
    assert( minor_5( "a" )  == "fis a cis" );

    assert( dominant_7( "c" )  == "g h d f" );
    assert( dominant_7( "d" )  == "a cis e g" );
    assert( dominant_7( "b" )  == "f a c es" );
    assert( dominant_7( "as" ) == "es g b des" );
    assert( dominant_7( "h" )  == "fis ais cis e" );

    return 0;
}
