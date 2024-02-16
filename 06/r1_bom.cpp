#include <cassert>
#include <string>
#include <memory>

/* TODO: do not use strings here */

/* Let's revisit the idea of a bill of materials that made a brief
 * appearance in ‹factory.cpp›, but in a slightly more useful
 * incarnation. */

/* Define the following class hierarchy: the base class, ‹part›,
 * should have a (pure) virtual method ‹description› that returns an
 * ‹std::string›. It should also keep an attribute of type
 * ‹std::string› and provide a getter for this attribute called
 * ‹part_no()› (part number). Then add 2 derived classes:
 *
 *  • ‹resistor› which takes the part number and an integral
 *    resistance as its constructor argument and provides a
 *    description of the form ‹"resistor ?Ω"› where ‹?› is the
 *    provided resistance,
 *
 *  • ‹capacitor› which also takes a part number and an integral
 *    capacitance and provides a description of the form
 *    ‹"capacitor ?μF"› where ‹?› is again the provided value. */

class part;
class resistor;
class capacitor;

/* We will also use owning pointers, so let us define a convenient
 * type alias for that: */

using part_ptr = std::unique_ptr< part >;

/* That was the mechanical part. Now we will need to think a bit: we
 * want a class ‹bom› which will remember a list of parts, along
 * with their quantities and will «own» the ‹part› instances it
 * holds. The interface:
 *
 *  • a method ‹add›, which accepts a ‹part_ptr› «by value» (it will
 *    take ownership of the instance) and the quantity (integer)
 *  • a method ‹find› which accepts an ‹std::string› and returns a
 *    ‹const› reference to the part instance with the given part
 *    number,
 *  • a method ‹qty› which returns the associated quantity, given a
 *    part number. */

class bom;

int main()
{
    bom list;
    const bom &clist = list;

    auto desc = [&]( std::string pn )
    {
        return clist.find( pn ).description();
    };

    auto qty = [&]( std::string pn ) { return clist.qty( pn ); };

    list.add( std::make_unique< resistor >( "r10k", 10000 ), 3 );
    assert( desc( "r10k" ) == "resistor 10000Ω" );
    assert( qty( "r10k" ) == 3 );

    list.add( std::make_unique< resistor >( "r2k", 2000 ), 1 );

    assert( desc( "r10k" ) == "resistor 10000Ω" );
    assert( desc( "r2k" ) == "resistor 2000Ω" );

    list.add( std::make_unique< capacitor >( "c200", 200 ), 8 );
    assert( desc( "r10k" ) == "resistor 10000Ω" );
    assert( desc( "r2k" ) == "resistor 2000Ω" );
    assert( desc( "c200" ) == "capacitor 200μF" );
    assert( qty( "c200" ) == 8 );

    return 0;
}
