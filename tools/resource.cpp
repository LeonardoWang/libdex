#include "dex.h"
#include "inst.h"

#include <cstdio>
#include <cstring>

int main(int argc, char * argv[])
{
    const char * dex_file;
    if (argc == 1)
        dex_file = "classes.dex";
    else
        dex_file = argv[1];

    Reader r = Reader::open_file(dex_file);
    auto dex = new Dex(r);

    int len = dex->strings.size();
    for (int i = 0; i < len; ++i) {
        if (!dex->strings[i].empty()) {
            const char *str = dex->strings[i].cstr();
            if (strncmp(str, "http://", 7) == 0 || strncmp(str, "https://", 8) == 0)
                printf("%s%c", str, '\0');
        }
    }

    return 0;
}
