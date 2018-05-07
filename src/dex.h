#pragma once

/** @file
 *  Definition of `Dex` class.
 **/

#include "dexstruct.h"

struct DexHeader;

/**
 *  The main class of this project, representing a DEX file
 *
 *  This class will load classes and fields definitions as well as methods declarations,
 *  but will not load the instructions.
 *
 *  To play with instructions, either use range-based for loops on `EncodedMethod`
 *  or use `split_basic_blocks()` function.
 *
 *  Refer following link for details:
 *      https://source.android.com/devices/tech/dalvik/dex-format
 **/
class Dex {
public:
    /** Load `mmap`-ed DEX file from disk **/
    Dex(Reader r) : r(r) { init(); }
    /** Load DEX from memory buffer **/
    Dex(const uint8_t * data) : r(data) { init(); }

    ~Dex();
    void munmap() { r.munmap(); }

    /** String literals **/
    vector<String> strings;
    /** Name of types **/
    vector<String> types;
    /** Signature of methods **/
    vector<Proto> protos;
    /** Class fields **/
    vector<Field> fields;
    /**
     *  Method declarations
     *  Use `Class::methods()` to get method definitions.
     **/
    vector<Method> methods;
    /** Classes **/
    vector<Class> classes;

    /** Check if an index of method delcartion is available **/
    bool has_method(int idx) const { return 0 <= idx && idx < (int) methods.size(); }

    struct FormatError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

private:
    Reader r;
    DexHeader * hdr;

    void init();
};
