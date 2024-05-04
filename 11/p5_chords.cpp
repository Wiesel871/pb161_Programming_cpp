#include <cassert>
#include <string>
#include <iostream>

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
const int lim = 1200;

enum type {
    bc, cc
};

enum ichord : int {
    c   = 0,    d   = 200,  e  = 400,   f   = 500,  g   = 700,  a   = 900,  h = 1100,
    cis = 100,  dis = 300,  /*eis,*/    fis = 600,  gis = 800,  ais = 1000,
    /* ces,*/   des = 100,  es = 300,   /*fes,*/    ges = 600,  as  = 800,  b = 1000
};

struct chord {
    int c;
    type t;

    chord(ichord c, type t) : c{c}, t{t} {}

    chord &operator-=(int i) {
        c -= i;
        c += lim;
        c %= lim;
        return *this;
    }

    chord &operator+=(int i) {
        c += i;
        c %= lim;
        return *this;
    }

};

const int sterc = 300, bterc = 400;

chord to_nchord(const std::string &key) {
    switch (key[0]) {
        case 'c': return {c, cc};
        case 'd': return {d, cc};
        case 'e': return {e, cc};
        case 'f': return {f, bc};
        case 'g': return {g, cc};
        case 'a': return {a, cc};
        case 'h': return {h, cc};
        case 'b': return {b, bc};
    }
    assert(false);
}

chord to_bchord(const std::string &key) {
    switch (key[0]) {
        case 'c': return {cis, cc};
        case 'd': return {dis, cc};
        case 'f': return {fis, cc};
        case 'g': return {gis, cc};
        case 'a': return {ais, cc};
    }
    assert(false);
}
chord to_cchord(const std::string &key) {
    switch (key[0]) {
        case 'd': return {des, bc};
        case 'e': return {es , bc};
        case 'g': return {ges, bc};
        case 'a': return {as , bc};
    }
    assert(false);
}

chord to_chord(const std::string &key) {
    if (key.size() == 1)
        return to_nchord(key);
    if (key[1] == 'i')
        return to_bchord(key);
    return to_cchord(key);
}

std::string from_chord(int ch) {
    switch (ch) {
        case f:     return "f";
        case c:     return "c";
        case d:     return "d";
        case e:     return "e";
        case g:     return "g";
        case a:     return "a";
        case h:     return "h";
    }
    assert(false);
}

std::string from_bchord(int ch) {
    switch (ch) {
        case b:     return "b";
        case des:   return "des";
        case es:    return "es";
        case ges:   return "ges";
        case as:    return "as";
    }
    return from_chord(ch);
}

std::string from_cchord(int ch) {
    switch (ch) {
        case cis:   return "cis";
        case dis:   return "dis";
        case fis:   return "fis";
        case gis:   return "gis";
        case ais:   return "ais";
    }
    return from_chord(ch);
}

std::string from_chord(chord ch) {
    if (ch.t == bc)
        return from_bchord(ch.c);
    return from_cchord(ch.c);
}


/* Durový kvintakord stavíme od základního tónu tóniny přidáním
 * velké tercie a další malé tercie, například c → c e g nebo e → e
 * gis h. */
std::string major_5( std::string key ) {
    auto c = to_chord(key);
    std::string res = key + ' ';
    c += bterc;
    res += from_chord(c) + ' ';
    c += sterc;
    res += from_chord(c);
    return res;
};

/* Mollový kvintakord stavíme od sexty (900 centů) paralelní durové
 * tóniny přidáním malé tercie a další velké tercie, např.
 * c → a c e, nebo e → cis e gis. */

std::string minor_5( std::string key ) {
    auto c = to_chord(key);
    std::string res;
    c -= sterc;
    res = from_chord(c) + ' ' + key + ' ';
    c += sterc + bterc;
    res += from_chord(c);
    return res;
};

/* Dominantní septakord stavíme od kvinty durové tóniny, např.
 * v tónině C bude postaven na tónu g, a vznikne přidáním jedné
 * velké a dvou malých tercií (celkem 4 tóny), například tedy
 * f → c e g b. */

std::string dominant_7( std::string key ) {
    auto c = to_chord(key);
    std::string res;
    c -= 500;
    res = from_chord(c) + ' ';
    c += bterc;
    res += from_chord(c) + ' ';
    c += sterc;
    res += from_chord(c) + ' ';
    c += sterc;
    res += from_chord(c) ;
    return res;
};

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

    assert( dominant_7( "d" )  == "a cis e g" );
    assert( dominant_7( "b" )  == "f a c es" );
    assert( dominant_7( "as" ) == "es g b des" );
    assert( dominant_7( "h" )  == "fis ais cis e" );
    assert( dominant_7( "c" )  == "g h d f" );

    return 0;
}
