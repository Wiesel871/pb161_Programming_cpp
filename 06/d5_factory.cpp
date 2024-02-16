#include <cassert>
#include <memory>
#include <sstream>

/* TODO: do not use strings here */

/* As we have seen, subtype polymorphism allows us to define an
 * «interface» in terms of ‹virtual› methods (that is, based on late
 * dispatch) and then create various «implementations» of this
 * interface.
 *
 * It is sometimes useful to create instances of multiple different
 * derived classes based on runtime inputs, but once they are
 * created, to treat them uniformly. The uniform treatment is made
 * possible by subtype polymorphism: if the entire interaction with
 * these objects is done through the shared interface, the instances
 * are all, at the type level, interchangeable with each other. The
 * behaviour of those instances will of course differ, depending on
 * their «dynamic type». */

/* When a system is designed this way, the entire program uses a
 * single «static type» to work with all instances from the given
 * inheritance hierarchy -- the type of the base class. Let's define
 * such a base class. */

class part
{
public:
    virtual std::string description() const = 0;
    virtual ~part() = default;
};

/* Let's add a simple function which operates on generic parts.
 * Working with instances is easy, since they can be passed through
 * a reference to the base type. For instance the following function
 * which formats a single line for a bill of materials (bom). */

std::string bom_line( const part &p, int count )
{
    return std::to_string( count ) + "x " + p.description();
}

/* However, «creation» of these instances poses a somewhat unique
 * challenge in C++: memory management. In languages like Java or
 * C#, we can create the instance and return a reference to the
 * caller, and the garbage collector will ensure that the instance
 * is correctly destroyed when it is no longer used. We do not have
 * this luxury in C++.
 *
 * Of course, we could always do memory management by hand, like
 * it's 1990. Fortunately, modern C++ provides «smart pointers» in
 * the standard library, making memory management much easier and
 * safer. Recall that a ‹unique_ptr› is an «owning» pointer: it
 * holds onto an object instance while it is in scope and destroys
 * it afterwards. Unlike objects stored in local variables, though,
 * the ownership of the instance held in a ‹unique_ptr› can be
 * transferred out of the function (i.e. an instance of ‹unique_ptr›
 * can be legally returned, unlike a reference to a local variable).
 *
 * This will make it possible to define a «factory»: a function
 * which constructs instances (parts) and returns them to the
 * caller. Of course, to actually define the function, we will need
 * to define the derived classes which it is supposed to create. */

using part_ptr = std::unique_ptr< part >;
part_ptr factory( std::string );

/* In the program design outlined earlier, the derived classes
 * change some of the behaviours, or perhaps add data members
 * (attributes) to the base class, but apart from construction, they
 * are entirely operated through the interface defined by the base
 * class. */

class cog : public part
{
    int teeth;
public:
    cog( int teeth ) : teeth( teeth ) {}

    std::string description() const override
    {
        return std::string( "cog with " ) +
               std::to_string( teeth ) + " teeth";
    }
};

class axle : public part
{
public:
    std::string description() const override
    {
        return "axle";
    }
};

class screw : public part
{
    int _thread, _length;
public:

    screw( int t, int l ) : _thread( t ), _length( l ) {}

    std::string description() const override
    {
        return std::to_string( _length ) + "mm M" +
               std::to_string( _thread ) + " screw";
    }
};

/* Now that we have defined the derived classes, we can finally
 * define the factory function. */

part_ptr factory( std::string desc )
{
    /* We will use ‹std::istringstream› (first described in
     * ‹06/streams.cpp›) to extract a description of the instance
     * that we want to create from a string. The format will be
     * simple: the type of the part, followed by its parameters
     * separated by spaces. */

    std::istringstream s( desc );
    std::string type;
    s >> type; /* extract the first word */

    if ( type == "cog" )
    {
        int teeth;
        s >> teeth;
        return std::make_unique< cog >( teeth );
    }

    if ( type == "axle" )
        return std::make_unique< axle >();

    if ( type == "screw" )
    {
        int thread, length;
        s >> thread >> length;
        return std::make_unique< screw >( thread, length );
    }

    throw std::runtime_error( "unexpected part description" );
}

int main() /* demo */
{
    /* Let's first use the factory to make some instances. They will
     * be held by ‹part_ptr› (i.e. ‹unique_ptr› with the static type
     * ‹part›. */

    part_ptr ax = factory( "axle" ),
             m7 = factory( "screw 7 50" ),
             m3 = factory( "screw 3 10" ),
             c8 = factory( "cog 8" ),
             c9 = factory( "cog 9" );

    /* From the point of view of the static type system, all the
     * parts created above are now the same. We can call the methods
     * which were defined in the interface, or we can pass them into
     * functions which work with parts. */

    assert( ax->description() == "axle" );
    assert( m7->description() == "50mm M7 screw" );
    assert( m3->description() == "10mm M3 screw" );
    assert( c8->description() == "cog with 8 teeth" );
    assert( c9->description() == "cog with 9 teeth" );

    /* Let's try using the ‹bom_line› function which we have defined
     * earlier. */

    assert( bom_line( *ax, 3 ) == "3x axle" );
    assert( bom_line( *m7, 20 ) == "20x 50mm M7 screw" );

    /* At the end of the scope, the objects are destroyed and all
     * memory is automatically freed. */
}
