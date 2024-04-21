#include <cstddef>
#include <cstdio>
#include <sstream>
#include <iostream>

int main() {
    std::ostringstream o;

    o << "#1 3\n\tabc 3 a b c\n\ta 0 abc\n\tb 0 abc\n\tc 0 abc\n#2 0\n\tabc 0";

    std::istringstream i(o.str());

    std::string id;
    std::size_t ends = 0;
    while (!i.eof()) {
        char mark = i.get();
        if (mark == '#') {
            std::size_t aux = 0;
            i >> aux;
            i >> ends;
            printf("%c %lu %lu\n", mark, aux, ends);
        } else if (mark == '\t') {
            i >> id;
            i >> ends;
            printf("%c | \'%s\' | %lu\n", mark, id.data(), ends);
            while (i.peek() == ' ') {
                std::string aux;
                i >> aux;
                printf("%c | \'%s\' | \'%s\'\n", mark, id.data(), aux.data());

            }
        }
    }

    return 0;
}
