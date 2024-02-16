#include <fstream>
#include <cassert>

/* This example will be brief: we will show how to open a file for
 * reading and fetch a line of text. We will then write that line of
 * text into a new file and read it back again to check that things
 * worked. */

/* We will split up the example into functions for 2 reasons:
 * first, to make it easier to follow, and second, to take advantage
 * of RAII: the file streams will close the underlying resource when
 * they are destroyed. In this case, that will be at the end of each
 * function. */

std::string read( const char *file )
{
    /* The default method of doing IO in C++ is through «streams».
     * Reading files is done through a stream of type
     * ‹std::ifstream›, which is short for «input file stream». The
     * constructor of ‹ifstream› takes the name of the file to open.
     * We will use a file given to us by the caller. */

    std::ifstream f( file );

    /* The simplest method to read text from a file is using
     * ‹std::getline›, which will fetch a single line at a time,
     * into an ‹std::string›. We need to prepare the string in
     * advance, since it is passed into ‹std::getline› as an output
     * argument. */

    std::string line;

    /* The ‹std::getline› function returns a reference to the stream
     * that was passed to it. Additionally, the stream can be
     * converted to ‹bool› to find out whether everything is okay
     * with it. If the reading fails for any reason, it will
     * evaluate to ‹false›. The newline character is discarded. */

    if ( !std::getline( f, line ) )

        /* In real code, we would of course want to handle errors,
         * because opening files is something that can fail for a
         * number of reasons. Here, we simply assume that everything
         * worked. */

        assert( false );

    return line;
}

/* Next comes a function which demonstrates writing into files. */

void write( const char *file, std::string line )
{
    /* To write data into a file, we can use ‹std::ofstream›, which
     * is short for «output file stream». The output file is created
     * if it does not exist. */

    std::ofstream f( file );

    /* Writing into a file is typically done using operators for
     * «formatted output». We will look at those in more detail in
     * the next section. For now, all we need to know that writing
     * an object into a stream is done like this: */

    f << line;

    /* We will also want to add the newline character that ‹getline›
     * above chomped. We have two options: either use the ‹"\n"›
     * string literal, or ‹std::endl› -- a so-called «stream
     * manipulator» which sends a newline character and asks the
     * stream to send the bytes to the operating system. Let's try
     * the more idiomatic approach, with the manipulator: */

    f << std::endl;

    /* At this point, the file is automatically closed and any
     * outstanding data is sent to the operating system. */
}

int main() /* demo */
{
    /* We first use ‹read› to get the first line of a random file. */

    std::string line = read( "zz.include.txt" );

    /* And we check that the line we got is what we expect. Remember
     * the stripped newline. */

    assert( line ==  "#ifdef foo" );

    /* Now we write the line into another file. After you run this
     * example, you can inspect ‹files.out› with an editor. It
     * should contain a copy of the first line of this file. */

    write( "d5_files.out", line );

    /* Finally, we use ‹read› again to read "file.out" back, and
     * check that the same thing came back. */

    std::string check = read( "d5_files.out" );
    assert( check == line );
}
