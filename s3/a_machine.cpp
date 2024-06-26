#include <cstdint>

/* V této úloze budete programovat jednoduchý registrový stroj
 * (model počítače). Stroj bude mít libovolný počet celočíselných
 * registrů a paměť adresovatelnou po bajtech. Registry jsou
 * indexované od 1 po ‹INT_MAX›. Každá instrukce jmenuje dva
 * registry a jednu přímo zakódovanou hodnotu (angl. immediate).
 *
 * V každém registru je uložena hodnota typu ‹int32_t›, tzn.
 * velikost strojového slova jsou 4 slabiky (bajty). V paměti jsou
 * slova uložena tak, že nejvýznamnější slabika má nejnižší adresu
 * (tzv. MSB-first). Počáteční hodnoty registrů i paměti jsou nuly.
 *
 * Stroj má následovné instrukce (kdykoliv je ‹reg_X› použito
 * v popisu, myslí se tím samotný registr – jeho hodnota nebo
 * úložišě – nikoliv jeho index; sloupec ‹reg_2› má opačný význam,
 * vztahuje se k indexu uloženému v instrukci).
 *
 *  │ opcode │‹reg_2›│ description                                 │
 *  ├────────┼───────┼◅────────────────────────────────────────────┤
 *  │ ‹mov›  │  ≥ 1  │ kopíruj hodnotu z ‹reg_2› do ‹reg_1›        │
 *  │        │  = 0  │ nastav ‹reg_1› na ‹immediate›               │
 *  │ ‹add›  │  ≥ 1  │ ulož ‹reg_1 + reg_2› do ‹reg_1›             │
 *  │        │  = 0  │ přičti ‹immediate› do ‹reg_1›               │
 *  │ ‹mul›  │  ≥ 1  │ ulož ‹reg_1 * reg_2› do ‹reg_1›             │
 *  │ ‹jmp›  │  = 0  │ skoč na adresu uloženou v ‹reg_1›           │
 *  │        │  ≥ 1  │ skoč na ‹reg_1› je-li ‹reg_2› nenulové      │
 *  │ ‹load› │  ≥ 1  │ načti hodnotu z adresy ‹reg_2› do ‹reg_1›   │
 *  │ ‹stor› │  ≥ 1  │ zapiš hodnotu ‹reg_1› na adresu ‹reg_2›     │
 *  │ ‹halt› │  = 0  │ zastav stroj s výsledkem ‹reg_1›            │
 *  │        │  ≥ 1  │ totéž, ale pouze je-li ‹reg_2› nenulový     │
 *
 * Každá instrukce je v paměti uložena jako 4 slova (adresy slov
 * rostou zleva doprava). Vykonání instrukce, která není skokem,
 * zvýší programový čítač o 4 slova.
 *
 *  ┌────────┬───────────┬───────┬───────┐
 *  │ opcode │ immediate │ reg_1 │ reg_2 │
 *  └────────┴───────────┴───────┴───────┘
 *
 * Vykonání jednotlivé instrukce smí zabrat nejvýše konstantní čas,
 * krom případů, kdy tato přistoupí k dosud nepoužité adrese nebo
 * registru. Paměť potřebná pro výpočet by měla být v nejhorším
 * případě úměrná součtu nejvyšší použité adresy a nejvyššího
 * použitého indexu registru. */

enum class opcode { mov, add, mul, jmp, load, stor, hlt };

struct machine
{
    /* Čtení a zápis paměti po jednotlivých slovech. */

    std::int32_t get( std::int32_t addr ) const;
    void         set( std::int32_t addr, std::int32_t v );

    /* Spuštění programu, počínaje adresou nula. Vrátí hodnotu
     * uloženou v ‹reg_1› zadaném instrukcí ‹hlt›, která výpočet
     * ukončila. */

    std::int32_t run();
};

int main()
{
    machine m;
    m.set( 0x00, static_cast< std::int32_t >( opcode::mov ) );
    m.set( 0x04, 7 );
    m.set( 0x08, 1 );
    m.set( 0x10, static_cast< std::int32_t >( opcode::hlt ) );
    m.set( 0x18, 1 );
    assert( m.run() == 7 );
    return 0;
}
