#pragma once

/** @file
 *  Unicode string.
 **/

#include <cstdint>
#include <cstring>
#include <stdexcept>

typedef uint32_t Char;

class String {
public:
    enum Encoding {
        UTF8,
        MUTF8,
    };

    String() { }
    String(const String & other);
    String(const char * str);
    String(const uint8_t * data, int len, int encoding);

    ~String();

    String & operator= (const String & str);

    Char operator[] (int idx) const;

    String slice(int l, int r) const;

    const char * c_str() const;
    const char * cstr() const { return c_str(); }

    bool empty() const { return !ctnt; }
    int size() const { return ctnt ? ctnt->len : 0; }
    int length() const { return size(); }

    int rfind(Char ch) const;

    struct EncodingError : public std::runtime_error {
        EncodingError(const char * msg);
        EncodingError(const char * msg, const uint8_t * data, int len);
    };

private:
    struct Content {
        int32_t ref_cnt;
        int32_t len;
        Char data[];
    };

    Content * ctnt = nullptr;

    String(Content * ctnt) : ctnt(ctnt) { }

    static Content * prep_ctnt(int len);
    void rel_ctnt(Content * ctnt);

    int check_idx(int idx) const;

    static Content * from_cstr(const char * str);
    static Content * from_utf8(const uint8_t * str, int len);
    static Content * from_mutf8(const uint8_t * str, int len);

    static Content * cat_ctnt(const Content * c1, const Content * c2);

    const char * to_utf8() const;

public:
    void debug_print() const;

    friend String operator+ (const String & s1, const String & s2);
    friend String operator+ (const String & s1, const char * s2);
    friend String operator+ (const char * s1, const String & s2);
};

String operator+ (const String & s1, const String & s2);
String operator+ (const String & s1, const char * s2);
String operator+ (const char * s1, const String & s2);
