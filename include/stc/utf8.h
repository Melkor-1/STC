#ifndef UTF8_H_INCLUDED
#define UTF8_H_INCLUDED
/*
// Example:
#include <stc/cstr.h>
#include <stc/csview.h>

int main()
{
    c_auto (cstr, s1) {
        s1 = cstr_new("hell😀 w😀rld");
        printf("%s\n", cstr_str(&s1));
        cstr_replace_sv(&s1, utf8_substr(cstr_str(&s1), 7, 1), c_sv("🐨"));
        printf("%s\n", cstr_str(&s1));

        csview sv = csview_from_s(s1);
        c_foreach (i, csview, sv)
            printf("%" c_PRIsv ",", c_ARGsv(i.codep));
    }
}
// Output:
// hell😀 w😀rld
// hell😀 w🐨rld
// h,e,l,l,😀, ,w,🐨,r,l,d,
*/
#include "ccommon.h"
#include <ctype.h>

enum { UTF8_OK = 0, UTF8_ERROR = 4 };
typedef struct { uint32_t state, codep, size; } utf8_decode_t;

/* encode/decode next utf8 codepoint. */
STC_API unsigned utf8_encode(char *out, uint32_t c);
STC_API uint32_t utf8_decode(utf8_decode_t *d, const uint8_t b);

/* number of codepoints in the utf8 string s */
STC_INLINE size_t utf8_size(const char *s) {
    size_t size = 0;
    while (*s) size += (*s++ & 0xC0) != 0x80;
    return size;
}

STC_INLINE size_t utf8_size_n(const char *s, size_t n) {
    size_t size = 0;
    while ((n-- != 0) & (*s != 0))
        size += (*s++ & 0xC0) != 0x80;
    return size;
}

STC_INLINE const char* utf8_at(const char *s, size_t index) {
    for (; (index > 0) & (*s != 0); ++s)
        index -= (s[1] & 0xC0) != 0x80;
    return s;
}

STC_INLINE const char* utf8_next(const char* s)
    { return utf8_at(s, 1); }

STC_INLINE size_t utf8_pos(const char* s, size_t index)
    { return utf8_at(s, index) - s; }

STC_INLINE uint32_t utf8_peek(const char *s) {
    utf8_decode_t d = {UTF8_OK, 0};
    const uint8_t* u = (const uint8_t*)s;
    utf8_decode(&d, *u++);
    switch (d.size) {
        case 4: utf8_decode(&d, *u++);
        case 3: utf8_decode(&d, *u++);
        case 2: utf8_decode(&d, *u++);
    }
    return d.codep;
}

STC_INLINE unsigned utf8_codep_size(const char *s) {
    const unsigned u = (uint8_t)*s;
    unsigned ret = (u & 0xF0) == 0xE0;
    ret += ret << 1;                       // 3
    ret |= u < 0x80;                       // 1
    ret |= ((0xC1 < u) & (u < 0xE0)) << 1; // 2
    ret |= ((0xEF < u) & (u < 0xF5)) << 2; // 4
    return ret;
}

STC_INLINE bool utf8_valid(const char* s) {
    utf8_decode_t d = {UTF8_OK, 0};
    const uint8_t* u = (const uint8_t *)s;
    while (*u) utf8_decode(&d, *u++);
    return d.state == UTF8_OK;
}

// --------------------------- IMPLEMENTATION ---------------------------------
#ifdef i_implement
// https://news.ycombinator.com/item?id=15423674
// https://gist.github.com/s4y/344a355f8c1f99c6a4cb2347ec4323cc

STC_DEF uint32_t utf8_decode(utf8_decode_t *d, const uint8_t b)
{
    switch (d->state) {
    case UTF8_OK:
        if      (b < 0x80) d->codep = b, d->size = 1;
        else if (b < 0xC2) d->state = UTF8_ERROR, d->size = 0;
        else if (b < 0xE0) d->state = 1, d->codep = b & 0x1F, d->size = 2;
        else if (b < 0xF0) d->state = 2, d->codep = b & 0x0F, d->size = 3;
        else if (b < 0xF5) d->state = 3, d->codep = b & 0x07, d->size = 4;
        else d->state = UTF8_ERROR, d->size = 0;
        break;
    case 1: case 2: case 3:
        if ((b & 0xC0) == 0x80) {
            d->state -= 1;
            d->codep = (d->codep << 6) | (b & 0x3F);
        } else
            d->state = UTF8_ERROR, d->size = 0;
    }
    return d->state;
}

STC_DEF unsigned utf8_encode(char *out, uint32_t c)
{
    if (c < 0x80U) {
        out[0] = (char) c;
        return 1;
    } else if (c < 0x0800U) {
        out[0] = (char) ((c>>6  & 0x1F) | 0xC0);
        out[1] = (char) ((c     & 0x3F) | 0x80);
        return 2;
    } else if (c < 0x010000U) {
        if (c < 0xD800U || c >= 0xE000U) {
            out[0] = (char) ((c>>12 & 0x0F) | 0xE0);
            out[1] = (char) ((c>>6  & 0x3F) | 0x80);
            out[2] = (char) ((c     & 0x3F) | 0x80);
            return 3;
        }
    } else if (c < 0x110000U) {
        out[0] = (char) ((c>>18 & 0x07) | 0xF0);
        out[1] = (char) ((c>>12 & 0x3F) | 0x80);
        out[2] = (char) ((c>>6  & 0x3F) | 0x80);
        out[3] = (char) ((c     & 0x3F) | 0x80);
        return 4;
    }
    return 0;
}

#endif
#endif
#undef i_opt
