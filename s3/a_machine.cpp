#include <cstdint>
#include <cassert>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>

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

struct machine;
enum class opcode { mov, add, mul, jmp, load, stor, hlt };

struct instruction {
    virtual void operator()(machine &, int32_t, int32_t, int32_t) const = 0;
};

struct add_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} add;
struct mov_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} mov;
struct mul_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} mul;
struct jmp_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} jmp;
struct load_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} load;
struct stor_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} stor;
struct hlt_i : public instruction {
    void operator()(machine&, int32_t, int32_t, int32_t) const override;
} hlt;

const std::unordered_map<int32_t, instruction *> insts {
    {static_cast<int32_t>(opcode::add), &add}, 
    {static_cast<int32_t>(opcode::mov), &mov},
    {static_cast<int32_t>(opcode::hlt), &hlt}, 
    {static_cast<int32_t>(opcode::jmp), &jmp},
    {static_cast<int32_t>(opcode::mul), &mul}, 
    {static_cast<int32_t>(opcode::load), &load},
    {static_cast<int32_t>(opcode::stor), &stor}
};

struct machine
{

    std::map<int32_t, int32_t> ram;
    std::map<int32_t, int32_t> registers;
    int32_t cur_addr = 0;
    /* Čtení a zápis paměti po jednotlivých slovech. */

    std::int32_t get( std::int32_t addr ) const {
        return ram.at(addr);
    }
    void set( std::int32_t addr, std::int32_t v ) {
        ram[addr] = v;
    }

    /* Spuštění programu, počínaje adresou nula. Vrátí hodnotu
     * uloženou v ‹reg_1› zadaném instrukcí ‹hlt›, která výpočet
     * ukončila. */

    std::int32_t run() {
        while (cur_addr >= 0) {
            int32_t opcode = ram[cur_addr];
            int32_t im = ram[cur_addr + 4];
            int32_t reg1 = ram[cur_addr + 8];
            int32_t reg2 = ram[cur_addr + 12];
            cur_addr += 16;
            (*insts.at(opcode))(*this, im, reg1, reg2);
        }
        return registers[1];
    };
};

void mov_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    if (reg2) {
        m.registers[reg1] = m.registers[reg2];
    } else {
        m.registers[reg1] = im;
    }
};
void add_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    if (reg2) {
        m.registers[reg1] += m.registers[reg2];
    } else {
        m.registers[reg1] += im;
    }
};

void jmp_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    (void) im;
    if (!reg2 || m.registers[reg2]) 
        m.cur_addr = m.registers[reg1];
};

void mul_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    if (reg2) {
        m.registers[reg1] *= m.registers[reg2];
    } else {
        m.registers[reg1] *= im;
    }
};

void load_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    (void) im;
    m.registers[reg1] = m.ram[m.registers[reg2]];
};

void stor_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    (void) im;
    m.ram[m.registers[reg2]] = m.registers[reg1];

};

void hlt_i::operator()(machine &m, int32_t im, int32_t reg1, int32_t reg2) const {
    (void) im;
    (void) reg1;
    if (!reg2 || m.registers[reg2]) 
        m.cur_addr = -1;
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
