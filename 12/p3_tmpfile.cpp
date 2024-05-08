#include <fstream>  /* fstream */
#include <cstdio>   /* remove */
#include <sstream>
#include <string>
#include <unistd.h> /* access */
#include <cassert>
#include <iostream>

/* We will implement a simple wrapper around ‹std::fstream› that
 * will act as a temporary file. When the object is destroyed, use
 * ‹std::remove› to unlink the file. Make sure the stream is closed
 * before you unlink the file. */

/* The ‹tmp_file› class should have the following interface:
 *
 *  • a constructor which takes the name of the file
 *  • method ‹write› which takes a string and replaces the content
 *    of the file with that string; this method should flush the
 *    data to the operating system (e.g. by closing the stream)
 *  • method ‹read› which returns the current content of the file
 *  • method ‹stream› which returns a reference to an instance of
 *    ‹std::fstream› (i.e. suitable for both reading and writing)
 *
 * Calling both ‹stream› and ‹write› on the same object is undefined
 * behaviour. The ‹read› method should return all data sent to the
 * file, including data written to ‹stream()› that was not yet
 * flushed by the user. */

class tmp_file {
    std::fstream file;
    std::string name;

    std::string consume() {
        auto pos = file.tellg();
        file.seekg(file.beg);
        std::string str(std::istreambuf_iterator<char>(file),{});
        file.seekg(pos);
        return str;
    }

    public:
    tmp_file(const std::string &str) : name(str) {
        file.open(str, std::fstream::out);
        file.close();
    }

    tmp_file(std::string &&str) : name(str) {
        file.open(str, std::fstream::out);
        file.close();
    }

    std::string read() {
        if (!file.is_open()) {
            std::string res;
            file.open(name, std::fstream::in);
            res = consume();
            file.close();
            return res;
        }
        return consume();
    }

    void write(const std::string &str) {
        file.open(name, std::fstream::out);
        file << str;
        file.close();
    }

    std::fstream &stream() {
        if (!file.is_open())
            file.open(name, std::fstream::in | std::fstream::out);
        return file;
    }

    ~tmp_file() {
        if (file.is_open())
            file.close();
        std::remove(name.data());
    }

};

int main()
{
    auto check_content = []( auto data )
    {
        std::ifstream ifs( "tmpfile.tmp" );
        std::string content;
        std::getline( ifs, content, '\0' );
        assert( content == data );
    };

    /* We use the POSIX function ‹access› to check file existence.
     * This is not part of the C++ standard and will only work on
     * UNIX-like systems. */

    std::remove( "tmpfile.tmp" );

    {
        assert( ::access( "tmpfile.tmp", F_OK ) );
        tmp_file t( "tmpfile.tmp" );
        assert( ::access( "tmpfile.tmp", F_OK ) == 0 );

        t.write( "hello world\n" );
        check_content( "hello world\n" );
        assert( t.read() == "hello world\n" );
    }

    assert( ::access( "tmpfile.tmp", F_OK ) );

    {
        tmp_file t( "tmpfile.tmp" );
        t.stream() << "bye world" << std::endl;
        check_content( "bye world\n" );
        t.stream() << "... bye";

        assert( t.read() == "bye world\n... bye" );
    }

    assert( ::access( "tmpfile.tmp", F_OK ) );

    return 0;
}
