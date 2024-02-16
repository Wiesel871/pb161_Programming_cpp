#include <sstream>
#include <cmath>
#include <cassert>

/* File streams are not the only kind of IO streams that are
 * available in the standard library. There are 3 ‘special’ streams,
 * called ‹std::cout›, ‹std::cerr› and ‹std::cin›. Those are not
 * types, but rather global variables, and represent the standard
 * output, the standard error output and the standard input of the
 * program. However, the first two are instances of ‹std::ostream›
 * and the third is an instance of ‹std::istream›.
 *
 * We don't know about class inheritance yet, but it is probably not
 * a huge stretch to understand that instances of ‹std::ofstream›
 * (output «file» stream) are also at the same time instances of
 * ‹std::ostream› (general output stream). The same story holds for
 * ‹std::ifstream› (input file stream) and ‹std::istream› (general
 * input stream).
 *
 * There is another pair of classes: ‹std::ostringstream› and
 * ‹std::istringstream›. Those streams are not attached to OS
 * resources, but to instances of ‹std::string›: in other words,
 * when you write to an ‹ostringstream›, the resulting bytes are not
 * sent to the operating system, but are instead appended to the
 * given string. Likewise, when you read from an ‹istringstream›,
 * the data is not pulled from the operating system, but instead
 * come from an ‹std::string›. Hopefully, you can see the
 * correspondence between files (the content of which are byte
 * sequences stored on disk) and strings (the content of which are
 * byte sequences stored in RAM).
 *
 * In any case, string streams are ideal for playing around, because
 * we can use the same tools as we always do: create some simple
 * instances, apply operations and use ‹assert› to check that the
 * results are what we expect. String-based streams are defined in
 * the header ‹sstream›. */

/* Everything that we will do with string streams applies to other
 * types of streams too (i.e. the 3 special streams mentioned
 * earlier, and all file streams).
 *
 * Like in the previous example, we will split up the demonstration
 * into a few sections, mainly to avoid confusion over variable
 * names. We will first demonstrate reading from streams. We have
 * already seen ‹std::getline›, so let's start with that. It is
 * probably noteworthy that it works on any input stream, not just
 * ‹std::ifstream›. */

void getline_1()
{

    std::istringstream istr( "a string\nwith 2 lines\n" );
    std::string s;

    assert( std::getline( istr, s ) );
    assert( s == "a string" );
    assert( std::getline( istr, s ) );
    assert( s == "with 2 lines" );
    assert( !std::getline( istr, s ) );
    assert( s.empty() );
}

/* We can also override the delimiter character for ‹std::getline›,
 * to extract delimited fields from input streams. */

void getline_2()
{
    std::istringstream istr( "colon:separated fields" );
    std::string s;

    assert( std::getline( istr, s, ':' ) );
    assert( s == "colon" );
    assert( std::getline( istr, s, ':' ) );
    assert( s == "separated fields" );
    assert( !std::getline( istr, s, ':' ) );
}

/* So far so good. Our other option is so-called «formatted input».
 * The standard library doesn't offer much in terms of ready-made
 * overloads for such inputs: there is one for strings, which
 * extracts individual words (like the ‹scanf› specifier ‹%s›, if
 * you remember that from C, but the C++ version is actually safe
 * and it is okay to use it). Then there is an instance for ‹char›,
 * which extracts a single character (regardless of whether it is a
 * whitespace character or not) and a bunch of overloads for various
 * numeric types. */

void formatted_input()
{
    std::istringstream istr( "integer 123 float 3.1415 s t" );
    std::string s, t;
    int i; float f;

    istr >> s; assert( s == "integer" );
    istr >> i; assert( i == 123 );
    istr >> s; assert( s == "float" );

    /* Notice that ‹float› numbers are not very exact. They are
     * usually just 32 bits, which means 24 bits of precision, which
     * is a bit less than 8 decimal digits. */

    istr >> f; assert( std::fabs( f - 3.1415 ) < 1e-7 );

    /* The last thing we want to demonstrate with regards to the
     * formatted input operators is that we can «chain» them. The
     * values are taken from left to right (behind the scenes, this
     * is achieved by the formatted input operator returning a
     * reference to its left operand. */

    istr >> s >> t;
    assert( s == "s" && t == "t" );

    /* When we reach the end of the stream (i.e. the end of the
     * buffer, or of the file), the stream will indicate an error. A
     * stream in error condition converts to ‹false› in a ‹bool›
     * context. */

    assert( !( istr >> s ) );
}

/* Output is actually quite a bit simpler than input. It is almost
 * always reasonable to use formatted output, since strings are
 * simply copied to the output without alterations. */

void formatted_output()
{
    std::ostringstream a, b, c;
    a << "hello world";

    /* To read the buffer associated with an output string stream,
     * we use its method ‹str›. Of course, this method is not
     * available on other stream types: in those cases, the
     * characters are written to files or to the terminal and we
     * cannot access them through the stream anymore. */

    assert( a.str() == "hello world" );

    /* Like with formatted input, output can be chained. */

    b << 123 << " " << 3.1415;
    assert( b.str() == "123 3.1415" );

    /* When writing delimited values to an output stream, it is
     * often desirable to only put the delimiter between items and
     * not after each item: this is an endless source of headaches.
     * Here is a trick to do it without too much typing: */

    int i = 0;
    for ( int v : { 1, 2, 3 } )
        c << ( i++ ? ", " : "" ) << v;

    assert( c.str() == "1, 2, 3" );
}

int main()
{
    getline_1();
    getline_2();
    formatted_input();
    formatted_output();
}
