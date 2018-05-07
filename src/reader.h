#pragma once

/** @file
 *  Binary stream reader.
 **/

#include "ustring.h"

#include <vector>
using std::vector;

/**
 *  Binary stream reader designed for DEX file
 **/
class Reader {
public:
    Reader(const uint8_t * data, int32_t off = 0, int size = -1)
        : data(data), off(off), size(size)
    { }

    /** Create a new reader on same buffer starts from given position **/
    Reader at(int32_t pos) { return Reader(data, pos, size); }

    /** Read a 16-bit unsigned int **/
    uint16_t u16();
    /** Read a 32-bit unsigned int **/
    uint32_t u32();
    /** Read a 64-bit unsigned int **/
    uint64_t u64();
    /** Read a uleb128 int **/
    uint32_t uleb128();
    /** Read a MUTF-8 string **/
    String str();
    /** Read n bytes **/
    vector<uint8_t> bytes(int n);
    /** Read a type list **/
    vector<String> type_list(const vector<String> & types);

    /** Get a pointer to current position **/
    const uint8_t * ptr() const { return data + off; }

    /** Seek to given position **/
    void seek(int32_t pos) { off = pos; }

    /** Check whether this reader is in valid state **/
    bool is_valid() { return off >= 0; }

    bool in_range(int64_t pos) { return pos < size - off; }

    void munmap();

    static Reader open_file(const char * file_name);

private:
    const uint8_t * data;
    int32_t off;
    int size;

    uint32_t read_utf8_2_bytes();
    uint32_t read_utf8_3_bytes();
};
