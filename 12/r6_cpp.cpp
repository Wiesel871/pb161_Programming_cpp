#include <iostream>
#include <string>
#include <cassert>

/* Implement a (very simplified) C preprocessor which supports
 * ‹#include "foo"› (without a search path, working directory only),
 * ‹#define› without a value, ‹#undef›, ‹#ifdef› and ‹#endif›. The
 * input is provided in a file, but the output should be returned as
 * a string.
 *
 * PS: Do not include line and filename information that ‹cpp›
 * normally adds to files. */

std::string cpp( const std::string &filename );

/* If you run this program with a parameter, it'll preprocess that
 * file and print the result to stdout. Feel free to experiment. */

int main( int argc, const char **argv )
{
    if ( argc >= 2 )
        std::cout << cpp( argv[ 1 ] );
    else
    {
        std::string actual_1 = cpp( "zz.preproc_1.txt" ),
                    expect_1 = "included foo\n"
                               "included bar\n"
                               "xoo\n"
                               "foo\n",
                    actual_2 = cpp( "zz.preproc_2.txt" ),
                    expect_2 = "included bar\n"
                               "included baz\n"
                               "included bar\n";

        assert( actual_1 == expect_1 );
        assert( actual_2 == expect_2 );
    }

    return 0;
}
