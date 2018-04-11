#pragma once

#include "dexstruct.h"

struct DexHeader;

/**
 *  The main class of this project, representing dex files.
 *
 *  This class will load classes and fields definitions as well as methods declarations,
 *  but will not load the instructions.
 *
 *  To play with instructions, use `Inst::load_method()`.
 *
 *  Refer following link for details:
 *      https://source.android.com/devices/tech/dalvik/dex-format
 **/
class Dex {
public:
    Dex(Reader r) : r(r) { init(); }
    Dex(const uint8_t * data) : r(data) { init(); }
    ~Dex();

    vector<String> strings;
    vector<String> types;
    vector<Proto> protos;
    vector<Field> fields;
    vector<Method> methods;
    vector<Class> classes;

    bool has_method(int idx) const { return 0 <= idx && idx < (int) methods.size(); }

    struct FormatError : public std::runtime_error
    {
        FormatError(const char * msg) : std::runtime_error(msg) { }
    };

private:
    Reader r;
    DexHeader * hdr;

    void init();
};
