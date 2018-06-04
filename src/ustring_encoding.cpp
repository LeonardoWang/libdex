#include "ustring.h"

namespace {

inline bool is_utf8_single_byte(uint8_t ch)    { return (ch & 0b1000'0000) == 0b0000'0000; }
inline bool is_utf8_following_byte(uint8_t ch) { return (ch & 0b1100'0000) == 0b1000'0000; }
inline bool is_utf8_2_bytes_header(uint8_t ch) { return (ch & 0b1110'0000) == 0b1100'0000; }
inline bool is_utf8_3_bytes_header(uint8_t ch) { return (ch & 0b1111'0000) == 0b1110'0000; }

inline Char decode_utf8_following_byte(uint8_t ch) { return (ch & 0b0011'1111); }
inline Char decode_utf8_2_bytes_header(uint8_t ch) { return (ch & 0b0001'1111); }
inline Char decode_utf8_3_bytes_header(uint8_t ch) { return (ch & 0b0000'1111); }

Char get_utf8_2_bytes(const uint8_t *& cur, const uint8_t * end)
{
    // 110x xxxx  10xx xxxx
    uint8_t one = *cur++;
    uint8_t two = cur < end ? *cur++ : 0;

    // no following bytes sanity check

    Char u1 = decode_utf8_2_bytes_header(one);
    Char u2 = decode_utf8_following_byte(two);
    return (u1 << 6) | u2;
}

Char get_utf8_3_bytes(const uint8_t *& cur, const uint8_t * end)
{
    // 1110 xxxx  10xx xxxx  10xx xxxx
    uint8_t one = *cur++;
    uint8_t two = cur < end ? *cur++ : 0;
    uint8_t three = cur < end ? *cur++ : 0;

    // no following bytes sanity check

    Char u1 = decode_utf8_3_bytes_header(one);
    Char u2 = decode_utf8_following_byte(two);
    Char u3 = decode_utf8_following_byte(three);
    return (u1 << 12) | (u2 << 6) | u3;
}

Char get_utf8_code_point(const uint8_t *& cur, const uint8_t * end)
{
    if (is_utf8_single_byte(*cur)) {
        return *cur++;

    } else if (is_utf8_2_bytes_header(*cur)) {
        return get_utf8_2_bytes(cur, end);

    } else if (is_utf8_3_bytes_header(*cur)) {
        return get_utf8_3_bytes(cur, end);

    } else {
        // discard this byte and return '?'
        ++cur;
        return '?';
    }
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

    int l = 0;
    const uint8_t *end = str + len;
    while (str < end)
        buf[l++] = get_utf8_code_point(str, end);

    Content * ret = prep_ctnt(l);
    for (int i = 0; i < l; ++i)
        ret->data[i] = buf[i];
    delete[] buf;
    return ret;
}

String::Content * String::from_mutf8(const uint8_t * str, int len)
{
    Char * buf = new Char[len];

    const uint8_t *cur = str, *end = str + len;
    int l = 0;

    while (cur < end) {
        Char ch = get_utf8_code_point(cur, end);
cont:
        if (is_utf16_surrogate_high(ch) && cur < end) {
            Char low = get_utf8_code_point(cur, end);
            if (! is_utf16_surrogate_low(low)) {
                buf[l++] = ch;
                ch = low;
                goto cont;
            } else {
                ch = get_utf16_surrogate_val(ch, low);
            }
        }

        buf[l++] = ch;
    }

    Content * ret = prep_ctnt(l);
    for (int i = 0; i < l; ++i)
        ret->data[i] = buf[i];
    delete[] buf;
    return ret;
}

namespace {
const char * new_empty_str()
{
    char * ret = new char[1];
    ret[0] = '\0';
    return ret;
}
}

const char * String::to_utf8() const
{
    if (! ctnt) return new_empty_str();

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
