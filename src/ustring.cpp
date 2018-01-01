#include "ustring.h"

String::EncodingError::EncodingError(const char * msg)
    : std::runtime_error(msg)
{ }

String::EncodingError::EncodingError(const char * msg, const uint8_t * data, int len)
    : std::runtime_error(msg)
{
    for (int i = 0; i < len; ++i)
        printf("%2x ", data[i]);
    putchar('\n');
}

String::String(const String & str)
{
    ctnt = str.ctnt;
    ctnt->ref_cnt += 1;
}

String::String(const char * str)
{
    if (str == nullptr) return;
    ctnt = from_cstr(str);
}

String::String(const uint8_t * data, int len, int encoding)
{
    switch (encoding) {
        case UTF8:  ctnt = from_utf8(data, len);  break;
        case MUTF8: ctnt = from_mutf8(data, len); break;
        default: throw EncodingError("unsupported encoding", data, len);
    }
}

String::~String()
{
    rel_ctnt(ctnt);
}

String & String::operator= (const String & str)
{
    if (ctnt == str.ctnt) return * this;
    rel_ctnt(ctnt);
    ctnt = str.ctnt;
    if (ctnt) ctnt->ref_cnt += 1;
    return * this;
}

Char String::operator[] (int idx) const
{
    return ctnt->data[check_idx(idx)];
}

String String::slice(int l, int r) const
{
    l = check_idx(l);
    r = check_idx(r);

    if (l >= r) return String();

    auto ret = prep_ctnt(r - l);
    std::copy(ctnt->data + l, ctnt->data + r, ret->data);
    return String(ret);
}

char * String::c_str() const
{
    return to_utf8();
}

int String::rfind(Char ch) const
{
    int ret = length() - 1;
    while (ret >= 0 && ctnt->data[ret] != ch)
        ret -= 1;
    return ret;
}

String::Content * String::cat_ctnt(const Content * c1, const Content * c2)
{
    Content * ret = prep_ctnt(c1->len + c2->len);
    std::copy(c1->data, c1->data + c1->len, ret->data);
    std::copy(c2->data, c2->data + c2->len, ret->data + c1->len);
    return ret;
}

String operator+ (const String & s1, const String & s2)
{
    if (! s1.ctnt) return s2;
    if (! s2.ctnt) return s1;
    return String(String::cat_ctnt(s1.ctnt, s2.ctnt));
}

String operator+ (const String & s1, const char * s2)
{
    if (! s2) return s1;
    if (! s1.ctnt) return String(s2);
    return String(String::cat_ctnt(s1.ctnt, String::from_cstr(s2)));
}

String operator+ (const char * s1, const String & s2)
{
    if (! s1) return s2;
    if (! s2.ctnt) return String(s1);
    return String(String::cat_ctnt(String::from_cstr(s1), s2.ctnt));
}

String::Content * String::prep_ctnt(int len)
{
    if (len == 0) return nullptr;
    auto ctnt = (Content *) new Char [2 + len];
    ctnt->ref_cnt = 1;
    ctnt->len = len;
    return ctnt;
}

void String::rel_ctnt(String::Content * ctnt)
{
    if (! ctnt) return;
    if (ctnt->ref_cnt == 1)
        delete [] (Char *) ctnt;
    else
        ctnt->ref_cnt -= 1;
};

int String::check_idx(int idx) const
{
    if (! ctnt)
        throw std::out_of_range("");
    if (idx < 0)
        idx = ctnt->len + idx;
    if (idx < 0 || idx >= ctnt->len)
        throw std::out_of_range("");
    return idx;
}

void String::debug_print() const
{
    if (! ctnt) { puts("empty"); return; }
    printf("[%d] ", ctnt->len);
    for (int i = 0; i < ctnt->len; ++i)
        printf("%8x ", ctnt->data[i]);
    putchar('\n');
}
