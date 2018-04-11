#include "ustring.h"

namespace {

inline bool is_utf8_single_byte(uint8_t ch)    { return (ch & 0b1000'0000) == 0b0000'0000; }
inline bool is_utf8_following_byte(uint8_t ch) { return (ch & 0b1100'0000) == 0b1000'0000; }
inline bool is_utf8_2_bytes_header(uint8_t ch) { return (ch & 0b1110'0000) == 0b1100'0000; }
inline bool is_utf8_3_bytes_header(uint8_t ch) { return (ch & 0b1111'0000) == 0b1110'0000; }

inline Char decode_utf8_following_byte(uint8_t ch) { return (ch & 0b0011'1111); }
inline Char decode_utf8_2_bytes_header(uint8_t ch) { return (ch & 0b0001'1111); }
inline Char decode_utf8_3_bytes_header(uint8_t ch) { return (ch & 0b0000'1111); }

Char get_utf8_2_bytes(const uint8_t * data)
{
    // 110x xxxx  10xx xxxx

    if (! is_utf8_following_byte(data[1]))
        throw String::EncodingError("not utf-8 2-bytes code point", data, 2);

    Char u1 = decode_utf8_2_bytes_header(data[0]);
    Char u2 = decode_utf8_following_byte(data[1]);
    return (u1 << 6) | u2;
}

Char get_utf8_3_bytes(const uint8_t * data)
{
    // 1110 xxxx  10xx xxxx  10xx xxxx

    if (! is_utf8_following_byte(data[1]) || ! is_utf8_following_byte(data[1]))
        throw String::EncodingError("not utf-8 3-bytes code point", data, 3);

    Char u1 = decode_utf8_3_bytes_header(data[0]);
    Char u2 = decode_utf8_following_byte(data[1]);
    Char u3 = decode_utf8_following_byte(data[2]);
    return (u1 << 12) | (u2 << 6) | u3;
}

Char get_utf8_code_point(const uint8_t * str, int * pp)
{
    int & p = * pp;

    if (is_utf8_single_byte(str[p])) {
        return str[p++];

    } else if (is_utf8_2_bytes_header(str[p])) {
        p += 2;
        return get_utf8_2_bytes(str + p - 2);

    } else if (is_utf8_3_bytes_header(str[p])) {
        p += 3;
        return get_utf8_3_bytes(str + p - 3);

    } else throw String::EncodingError("not utf-8 code point", str, 4);
}

bool is_utf16_surrogate_high(Char ch) { return (ch & 0b1111'1100'0000'0000) == 0b1101'1000'0000'0000; }
bool is_utf16_surrogate_low(Char ch)  { return (ch & 0b1111'1100'0000'0000) == 0b1101'1100'0000'0000; }

Char get_utf16_surrogate_val(Char h, Char l)
{
    h &= 0b0000'0011'1111'1111;
    l &= 0b0000'0011'1111'1111;
    return (h << 10) + l + 0x1'0000;
}

}

String::Content * String::from_cstr(const char * str)
{
    return from_utf8((const uint8_t *) str, strlen(str));
}

String::Content * String::from_utf8(const uint8_t * str, int len)
{
    Char * buf = new Char[len];

    int i = 0;
    int l = 0;
    while (i < len)
        buf[l++] = get_utf8_code_point(str, & i);
    if (i != len) throw EncodingError("incomplete utf8 string");

    Content * ret = prep_ctnt(l);
    for (int i = 0; i < l; ++i)
        ret->data[i] = buf[i];
    delete[] buf;
    return ret;
}

String::Content * String::from_mutf8(const uint8_t * str, int len)
{
    Char * buf = new Char[len];

    int i = 0;
    int l = 0;
    while (i < len) {
        Char ch = get_utf8_code_point(str, & i);

        if (is_utf16_surrogate_high(ch)) {
            Char low = get_utf8_code_point(str, & i);
            if (! is_utf16_surrogate_low(low)) {
                buf[l++] = ch;
                ch = low;
            } else {
                ch = get_utf16_surrogate_val(ch, low);
            }
        }

        buf[l++] = ch;
    }

    if (i != len) throw EncodingError("incomplete mutf8 string");

    Content * ret = prep_ctnt(l);
    for (int i = 0; i < l; ++i)
        ret->data[i] = buf[i];
    delete[] buf;
    return ret;
}

char * String::to_utf8() const
{
    int len = 0;
    for (int i = 0; i < ctnt->len; ++i) {
        len += 1;
        if (ctnt->data[i] > 0b0000'0000'0111'1111) len += 1;
        if (ctnt->data[i] > 0b0000'0111'1111'1111) len += 1;
        if (ctnt->data[i] > 0b1111'1111'1111'1111) len += 1;
    }

    char * ret = new char[len + 1];

    int p = 0;
    for (int i = 0; i < ctnt->len; ++i) {
        Char ch = ctnt->data[i];
        if (ch <= 0b0111'1111) {
            ret[p++] = ch & 0b0111'1111;
        } else if (ch <= 0b0000'0111'1111'1111) {
            ret[p++] = ((ch & 0b0000'0111'1100'0000) >> 6) | 0b1100'0000;
            ret[p++] =  (ch & 0b0011'1111)                 | 0b1000'0000;
        } else if (ch <= 0b1111'1111'1111'1111) {
            ret[p++] = ((ch & 0b1111'0000'0000'0000) >> 12) | 0b1110'0000;
            ret[p++] = ((ch & 0b0000'1111'1100'0000) >> 6)  | 0b1000'0000;
            ret[p++] =  (ch & 0b0000'0000'0011'1111)        | 0b1000'0000;
        } else {
            ret[p++] = ((ch & 0b0001'1100'0000'0000'0000'0000) >> 18) | 0b1111'0000;
            ret[p++] = ((ch & 0b0000'0011'1111'0000'0000'0000) >> 12) | 0b1000'0000;
            ret[p++] = ((ch & 0b0000'0000'0000'1111'1100'0000) >> 6)  | 0b1000'0000;
            ret[p++] =  (ch & 0b0000'0000'0000'0000'0011'1111)        | 0b1000'0000;
        }
    }

    ret[len] = 0;
    return ret;
}
