
/*
 * Copyright (C) Niklaus F.Schen.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "mln_regexp.h"

static int mln_match_star(char *mregexp, int mreglen, \
                          char *regexp, char *text, \
                          int reglen, int textlen);
static int mln_match_here(unsigned int flag, \
                          char *regexp, char *text, \
                          int reglen, int textlen);
static int mln_match_plus(char *mregexp, int mreglen, \
                          char *regexp, char *text, \
                          int reglen, int textlen);
static int mln_match_question(char *mregexp, int mreglen, \
                              char *regexp, char *text, \
                              int reglen, int textlen);
static int mln_match_brace(char *mregexp, int mreglen, \
                           char *regexp, char *text, \
                           int reglen, int textlen, \
                           int min, int max);
static inline int
mln_match_square(char *regexp, int reglen, char **text, int *textlen);
static inline void
mln_match_get_limit(char *regexp, int reglen, int *min, int *max);
static inline int mln_get_char(unsigned int flag, char *s, int len);
static inline int mln_get_length(char *s, int len);
static inline int
mln_process_or(unsigned int flag, \
               char **regexp, int *reglen, \
               char **text,   int *textlen);
static int
mln_or_return_val(char **regexp, int *reglen, char *rexp, int rlen, int rv);
static inline void
mln_adjust_or_pos(unsigned int flag, char **rexp, int *rlen);

static int mln_match_here(unsigned int flag, \
                          char *regexp, char *text, \
                          int reglen, int textlen)
{
    int steplen, count, c_0, len_0, c_n, len_n, ret;

again:
    c_0 = mln_get_char(flag, regexp, reglen);
    len_0 = mln_get_length(regexp, reglen);
    steplen = len_0;

    if (reglen < 1) {
        return textlen;
    }

    if (!(flag & M_REGEXP_SPECIAL_MASK)) {
        if (!(flag & M_REGEXP_MASK_OR)) {
            ret = mln_process_or(flag, &regexp, &reglen, &text, &textlen);
            if (ret < 0) return -1;
            else if (ret > 0) goto again;
        }

        if (c_0 == M_REGEXP_LPAR) {
            int c, len = reglen;
            count = 0;
            while (len > 0) {
                c = mln_get_char(flag, regexp+(reglen-len), len);
                if (c == M_REGEXP_LPAR) count++;
                if (c == M_REGEXP_RPAR && --count == 0) break;
                len -= mln_get_length(regexp+(reglen-len), len);
            }
            if (len <= 0) return -1;
            steplen = reglen - len + len_0;
            if (reglen - len == len_0) {
                regexp += (len_0 << 1);
                reglen -= (len_0 << 1);
                goto again;
            }
        }

        if (c_0 == M_REGEXP_LSQUAR) {
            int c, len = reglen;
            count = 0;
            while (len > 0) {
                c = mln_get_char(flag, regexp+(reglen-len), len);
                if (c == M_REGEXP_LSQUAR) count++;
                if (c == M_REGEXP_RSQUAR && --count == 0) break;
                len -= mln_get_length(regexp+(reglen-len), len);
            }
            if (len <= 0) return -1;
            steplen = reglen - len + len_0;
            if (reglen - len == len_0) {
                regexp += (len_0 << 1);
                reglen -= (len_0 << 1);
                goto again;
            }
        }

        if (steplen < reglen) {
            c_n = mln_get_char(flag, regexp+steplen, reglen-steplen);
            len_n = mln_get_length(regexp+steplen, reglen-steplen);

            if (c_n == M_REGEXP_STAR) {
                return mln_match_star(regexp, steplen, \
                                      regexp+steplen+len_n, text, \
                                      reglen-steplen-len_n, textlen);
            }
            if (c_n == M_REGEXP_PLUS) {
                return mln_match_plus(regexp, steplen, \
                                      regexp+steplen+len_n, text, \
                                      reglen-steplen-len_n, textlen);
            }
            if (c_n == M_REGEXP_QUES) {
                return mln_match_question(regexp, steplen, \
                                          regexp+steplen+len_n, text, \
                                          reglen-steplen-len_n, textlen);
            }
            if (c_n == M_REGEXP_LBRACE) {
                int part = 1, min, max, existent = 0;
                int c, len = reglen, l = mln_get_length(regexp+steplen, reglen-steplen);
                count = 0;
                while (len > steplen) {
                    c = mln_get_char(flag, regexp+steplen+(reglen-len), reglen-steplen-(reglen-len));
                    if (c == ',') {
                        part++;
                        len -= mln_get_length(regexp+steplen+(reglen-len), reglen-steplen-(reglen-len));
                        continue;
                    }
                    if (c == M_REGEXP_LBRACE) {
                        count++;
                        len -= mln_get_length(regexp+steplen+(reglen-len), reglen-steplen-(reglen-len));
                        continue;
                    }
                    if (c == M_REGEXP_RBRACE) {
                        if (--count == 0) break;
                        len -= mln_get_length(regexp+steplen+(reglen-len), reglen-steplen-(reglen-len));
                        continue;
                    }
                    if (!isdigit(c)) return -1;
                    existent = 1;
                    len -= mln_get_length(regexp+steplen+(reglen-len), reglen-steplen-(reglen-len));
                }
                if (len <= steplen || !existent || part > 2) return -1;
                mln_match_get_limit(regexp+steplen+l, reglen-len-l, &min, &max);
                if (max > 0 && min > max) return -1;
                return mln_match_brace(regexp, steplen, \
                                       regexp+steplen+(reglen-len)+l, text, \
                                       len-steplen-l, textlen, \
                                       min, max);
            }
        }

        if (c_0 == M_REGEXP_XOR && (flag & M_REGEXP_MASK_NEW)) {
            regexp += len_0;
            reglen -= len_0;
            goto again;
        }
        flag &= ~M_REGEXP_MASK_NEW;
        if (c_0 == M_REGEXP_DOLL) {
            return textlen < 1? textlen: -1;
        }
        if (flag & M_REGEXP_MASK_SQUARE && reglen > len_0 && textlen > 0) {
            int sub_c, sub_len;
            sub_c = mln_get_char(flag, regexp+len_0, reglen-len_0);
            sub_len = mln_get_length(regexp+len_0, reglen-len_0);
            if (sub_c == M_REGEXP_SUB && reglen > len_0+sub_len) {
                sub_c = mln_get_char(flag, regexp+len_0+sub_len, reglen-len_0-sub_len);
                sub_len = mln_get_length(regexp+len_0+sub_len, reglen-len_0-sub_len);
                if (text[0] >= c_0 && text[0] <= sub_c) {
                    return --textlen;
                }
                return -1;
            }
        }
        if (c_0 == M_REGEXP_NUM && textlen > 0) {
            if (!isdigit(*text)) return -1;
            text++;
            textlen--;
            regexp += len_0;
            reglen -= len_0;
            goto again;
        }
        if (c_0 == M_REGEXP_NOT_NUM && textlen > 0) {
            if (isdigit(*text)) return -1;
            text++;
            textlen--;
            regexp += len_0;
            reglen -= len_0;
            goto again;
        }
        if (c_0 == M_REGEXP_ALPHA && textlen > 0) {
            if (!isalpha(*text)) return -1;
            text++;
            textlen--;
            regexp += len_0;
            reglen -= len_0;
            goto again;
        }
        if (c_0 == M_REGEXP_DOT && textlen > 0) {
            regexp += len_0;
            text++;
            reglen -= len_0;
            textlen--;
            goto again;
        }
    }

    if (c_0 == M_REGEXP_LSQUAR) {
        if (mln_match_square(regexp, steplen, &text, &textlen) < 0)
            return -1;
        regexp += steplen;
        reglen -= steplen;
        goto again;
    }

    if (c_0 == M_REGEXP_LPAR) {
        int left = mln_match_here(M_REGEXP_MASK_NEW, regexp+len_0, text, steplen-(len_0<<1), textlen);
        if (left < 0) return -1;
        regexp += steplen;
        reglen -= steplen;
        text += (textlen - left);
        textlen = left;
        goto again;
    }

    if (textlen > 0 && c_0 == (unsigned char)(text[0])) {
        if (flag & M_REGEXP_SPECIAL_MASK) return textlen;
        regexp += len_0;
        text++;
        reglen -= len_0;
        textlen--;
        goto again;
    }

    return -1;
}

static inline int
mln_process_or(unsigned int flag, \
               char **regexp, int *reglen, \
               char **text,   int *textlen)
{
    char *rexp = *regexp;
    int rlen = *reglen;
    int c, len, left, count, ret, last = 0;
    int loop = 0, rv = 0, match_len;

again:
    c = mln_get_char(flag, rexp, rlen);
    len = mln_get_length(rexp, rlen);

    left = rlen;
    count = 0;
    if (c == M_REGEXP_LSQUAR) {
        while (left > 0) {
            c = mln_get_char(flag, rexp+(rlen-left), left);
            left -= mln_get_length(rexp+(rlen-left), left);
            if (c == M_REGEXP_LSQUAR) count++;
            if (c == M_REGEXP_RSQUAR && --count == 0) break;
        }
    } else if (c == M_REGEXP_LPAR) {
        while (left > 0) {
            c = mln_get_char(flag, rexp+(rlen-left), left);
            left -= mln_get_length(rexp+(rlen-left), left);
            if (c == M_REGEXP_LPAR) count++;
            if (c == M_REGEXP_RPAR && --count == 0) break;
        }
    } else {
        if (last == 1 && (c == M_REGEXP_OR || rlen <= 0)) {
            if (rlen > 0) {
                rexp += len;
                rlen -= len;
            }
            mln_adjust_or_pos(flag, &rexp, &rlen);
            return mln_or_return_val(regexp, reglen, rexp, rlen, rv);
        }
        left -= len;
    }

    last = 0;

    if (left <= 0) {
        if (loop == 0) return mln_or_return_val(regexp, reglen, rexp, rlen, rv);
        loop = 0;
        match_len = rlen;
        goto match;
    }

    c = mln_get_char(flag, rexp+(rlen-left), left);
    len = mln_get_length(rexp+(rlen-left), left);
    if (c == M_REGEXP_STAR || c == M_REGEXP_QUES || c == M_REGEXP_PLUS) {
        left -= len;
        c = mln_get_char(flag, rexp+(rlen-left), left);
        len = mln_get_length(rexp+(rlen-left), left);
    } else if (flag & M_REGEXP_MASK_SQUARE && c == M_REGEXP_SUB) {
        left -= len;
        left -= mln_get_length(rexp+(rlen-left), left);
        c = mln_get_char(flag, rexp+(rlen-left), left);
        len = mln_get_length(rexp+(rlen-left), left);
    } else if (c == M_REGEXP_LBRACE) {
        while (left > 0) {
            c = mln_get_char(flag, rexp+(rlen-left), left);
            left -= mln_get_length(rexp+(rlen-left), left);
            if (c == M_REGEXP_LBRACE) count++;
            if (c == M_REGEXP_RBRACE && --count == 0) break;
        }
        c = mln_get_char(flag, rexp+(rlen-left), left);
        len = mln_get_length(rexp+(rlen-left), left);
    }

    if (c != M_REGEXP_OR) {
        if (loop == 0) return mln_or_return_val(regexp, reglen, rexp, rlen, rv);
        loop = 0;
        match_len = left > 0? rlen-left: rlen;
        goto match;
    }

    last = loop = rv = 1;
    match_len = rlen - left;

match:
    ret = mln_match_here(flag|M_REGEXP_MASK_OR, rexp, *text, match_len, *textlen);

    rexp += match_len;
    rlen -= match_len;
    if (left > 0 && c == M_REGEXP_OR) {
        rexp += len;
        rlen -= len;
    }

    if (ret >=0 ) {
        mln_adjust_or_pos(flag, &rexp, &rlen);
        (*text) += (*textlen - ret);
        (*textlen) = ret;
        return mln_or_return_val(regexp, reglen, rexp, rlen, rv);
    }

    if (loop) goto again;
    return mln_or_return_val(regexp, reglen, rexp, rlen, -1);
}

static int
mln_or_return_val(char **regexp, int *reglen, char *rexp, int rlen, int rv)
{
    *regexp = rexp;
    *reglen = rlen;
    return rv;
}

static inline void
mln_adjust_or_pos(unsigned int flag, char **rexp, int *rlen)
{
    int c, len, count;

again:
    if (*rlen == 0) return;

    c = mln_get_char(flag, *rexp, *rlen);
    len = mln_get_length(*rexp, *rlen);

    count = 0;
    if (c == M_REGEXP_LSQUAR) {
        while (*rlen > 0) {
            c = mln_get_char(flag, *rexp, *rlen);
            len = mln_get_length(*rexp, *rlen);
            (*rexp) += len;
            (*rlen) -= len;
            if (c == M_REGEXP_LSQUAR) count++;
            if (c == M_REGEXP_RSQUAR && --count == 0) break;
        }
    } else if (c == M_REGEXP_LPAR) {
        while (*rlen > 0) {
            c = mln_get_char(flag, *rexp, *rlen);
            len = mln_get_length(*rexp, *rlen);
            (*rexp) += len;
            (*rlen) -= len;
            if (c == M_REGEXP_LPAR) count++;
            if (c == M_REGEXP_RPAR && --count == 0) break;
        }
    } else {
        (*rexp) += len;
        (*rlen) -= len;
    }

    if (*rlen <= 0) return;

    c = mln_get_char(flag, *rexp, *rlen);
    len = mln_get_length(*rexp, *rlen);

    if (c == M_REGEXP_STAR || c == M_REGEXP_QUES || c == M_REGEXP_PLUS) {
        (*rexp) += len;
        (*rlen) -= len;
        if (*rlen <= 0) return;
        c = mln_get_char(flag, *rexp, *rlen);
        len = mln_get_length(*rexp, *rlen);
    } else if (flag & M_REGEXP_MASK_SQUARE && c == M_REGEXP_SUB) {
        (*rexp) += len;
        (*rlen) -= len;
        if (*rlen <= 0) return;
        len = mln_get_length(*rexp, *rlen);
        (*rexp) += len;
        (*rlen) -= len;
        if (*rlen <= 0) return;
        c = mln_get_char(flag, *rexp, *rlen);
        len = mln_get_length(*rexp, *rlen);
    } else if (c == M_REGEXP_LBRACE) {
        while (*rlen > 0) {
            c = mln_get_char(flag, *rexp, *rlen);
            len = mln_get_length(*rexp, *rlen);
            (*rexp) += len;
            (*rlen) -= len;
            if (c == M_REGEXP_LPAR) count++;
            if (c == M_REGEXP_RPAR && --count == 0) break;
        }
        if (*rlen <= 0) return;
        c = mln_get_char(flag, *rexp, *rlen);
        len = mln_get_length(*rexp, *rlen);
    }

    if (c == M_REGEXP_OR) {
        (*rexp) += len;
        (*rlen) -= len;
        goto again;
    }
}

static inline int
mln_match_square(char *regexp, int reglen, char **text, int *textlen)
{
    int c, len, reverse = 0, count, left, steplen;
    int end_c, tmp_c, tmp_len;

    /*jump off '['*/
    len = mln_get_length(regexp, reglen);
    regexp += len;
    reglen -= len;

    c = mln_get_char(M_REGEXP_MASK_SQUARE, regexp, reglen);
    len = mln_get_length(regexp, reglen);
    if (c == M_REGEXP_XOR) {
        reverse = 1;
        regexp += len;
        reglen -= len;
    }

again:
    while (reglen > 0) {
        c = mln_get_char(M_REGEXP_MASK_SQUARE, regexp, reglen);
        len = mln_get_length(regexp, reglen);

        if (c == M_REGEXP_RSQUAR) break;

        switch (c) {
            case M_REGEXP_LBRACE:
                end_c = M_REGEXP_RBRACE;
                break;
            case M_REGEXP_LPAR:
                end_c = M_REGEXP_RPAR;
                break;
            case M_REGEXP_LSQUAR:
                end_c = M_REGEXP_RSQUAR;
                break;
            default:
                if (c > M_REGEXP_SEPARATOR) {
                    regexp += len;
                    reglen -= len;
                    goto again;
                }
                end_c = 0;
                break;
        }

        if (end_c == 0) {
            steplen = len;
            if (reglen > len) {
                int sub_c, sub_len;
                sub_c = mln_get_char(M_REGEXP_MASK_SQUARE, regexp+len, reglen-len);
                sub_len = mln_get_length(regexp+len, reglen-len);
                if (sub_c == M_REGEXP_SUB && reglen > len+sub_len) {
                    steplen += (sub_len + mln_get_length(regexp+len+sub_len, reglen-len-sub_len));
                }
            }
        } else {
            steplen = -1;
            count = 0;
            tmp_len = reglen;
            while (tmp_len > 1) {
                tmp_c = mln_get_char(M_REGEXP_MASK_SQUARE, regexp+(reglen-tmp_len), tmp_len);
                tmp_len -= mln_get_length(regexp+(reglen-tmp_len), tmp_len);
                if (tmp_c == c) count++;
                if (tmp_c == end_c && --count == 0) {
                    steplen = reglen - tmp_len;
                    break;
                }
            }
            if (steplen < 0) return -1;
            if (steplen == (len << 1)) {
                regexp += steplen;
                reglen = tmp_len;
                continue;
            }
        }

        if (*textlen <= 0) return -1;
        left = mln_match_here(M_REGEXP_MASK_SQUARE, regexp, *text, steplen, *textlen);
        if (left >= 0) {
            if (!reverse) {
                (*text) += (*textlen - left);
                *textlen = left;
                return left;
            } else {
                return -1;
            }
        }

        regexp += steplen;
        reglen -= steplen;
    }
 
    if (reverse) {
        if (*textlen <= 0) return -1;
        (*textlen)--;
        (*text)++;
        return *textlen;
    }
    if (*textlen <= 0) return *textlen;
    return -1;
}

static inline void
mln_match_get_limit(char *regexp, int reglen, int *min, int *max)
{
    int val;
    char *p, *end = regexp + reglen;

    for (val = 0, p = regexp; p < end && *p != ','; p++) {
        val += (val * 10 + (*p - '0'));
    }
    *min = *max = val;

    if (p >= end) return;

    *max = -1;
    for (val = 0, p++; p < end; p++) {
        val += (val * 10 + (*p - '0'));
    }
    if (val == 0) return;

    *max = val;
}

static int mln_match_star(char *mregexp, int mreglen, \
                          char *regexp, char *text, \
                          int reglen, int textlen)
{
    int ret;
    char dot = (char)M_REGEXP_DOT;

    if (textlen <= 0) return textlen;

    if (mreglen > 1) {
again:
        ret = mln_match_here(0, mregexp, text, mreglen, textlen);
        if (ret < 0) {
            if (reglen <= 0) return ret;
            return mln_match_here(0, regexp, text, reglen, textlen);
        } else {
            text += (textlen - ret);
            textlen = ret;
            if (textlen > 0) goto again;
            if (reglen > 0) return mln_match_here(0, regexp, text, reglen, textlen);
            return 0;
        }
    }

    while (textlen > 0 && \
           (mln_match_here(M_REGEXP_STAR, mregexp, text, mreglen, textlen) >= 0 || \
            mln_match_here(M_REGEXP_STAR, mregexp, &dot, mreglen, 1) >= 0))
    {
        text++;
        textlen--;
    }

    if (reglen > 0) {
        return mln_match_here(0, regexp, text, reglen, textlen);
    }

    return -1;
}

static int mln_match_plus(char *mregexp, int mreglen, \
                          char *regexp, char *text, \
                          int reglen, int textlen)
{
    int ret, found = 0;
    char dot = (char)M_REGEXP_DOT;

    if (mreglen > 1) {
again:
        ret = mln_match_here(0, mregexp, text, mreglen, textlen);
        if (ret < 0) {
            if (found == 0) return ret;
            return mln_match_here(0, regexp, text, reglen, textlen);
        } else {
            found = 1;
            text += (textlen - ret);
            textlen = ret;
            if (textlen > 0) goto again;
            if (reglen > 0) return mln_match_here(0, regexp, text, reglen, textlen);
            return 0;
        }
    }

    while (textlen > 0 && \
           (mln_match_here(M_REGEXP_PLUS, mregexp, text, mreglen, textlen) >= 0 || \
            mln_match_here(M_REGEXP_PLUS, mregexp, &dot, mreglen, 1) >= 0))
    {
        found = 1;
        text++;
        textlen--;
    }
    if (found) {
        if (textlen > 0)
            return mln_match_here(0, regexp, text, reglen, textlen);
        return textlen;
    }

    return -1;
}

static int mln_match_question(char *mregexp, int mreglen, \
                              char *regexp, char *text, \
                              int reglen, int textlen)
{
    int ret;

    if (mreglen > 1) {
        ret = mln_match_here(0, mregexp, text, mreglen, textlen);
        if (ret >= 0) {
            text += (textlen - ret);
            textlen = ret;
        }
        return mln_match_here(0, regexp, text, reglen, textlen);
    }

    if (mln_match_here(M_REGEXP_QUES, mregexp, text, mreglen, textlen) >= 0)
        return mln_match_here(0, regexp, text+1, reglen, textlen-1);
    return mln_match_here(0, regexp, text, reglen, textlen);
}

static int mln_match_brace(char *mregexp, int mreglen, \
                           char *regexp, char *text, \
                           int reglen, int textlen, \
                           int min, int max)
{
    int ret, found = 0;
    char dot = (char)M_REGEXP_DOT;

    if (mreglen > 1) {
again:
        ret = mln_match_here(0, mregexp, text, mreglen, textlen);
        if (ret < 0) {
            if (reglen <= 0 || found < min) return ret;
            return mln_match_here(0, regexp, text, reglen, textlen);
        } else {
            found++;
            text += (textlen - ret);
            textlen = ret;
            if (textlen > 0 && (max < 0 || found < max)) goto again;
            if (textlen <= 0 && reglen <= 0) return 0;
            return mln_match_here(0, regexp, text, reglen, textlen);
        }
    }

    while (textlen > 0 && \
           (mln_match_here(M_REGEXP_LBRACE, mregexp, text, mreglen, textlen) >= 0 || \
            mln_match_here(M_REGEXP_LBRACE, mregexp, &dot, mreglen, 1) >= 0))
    {
        found++;
        text++;
        textlen--;
        if (max > 0 && found >= max) break;
    }
    if (found >= min) {
        if (textlen > 0 || reglen > 0)
            return mln_match_here(0, regexp, text, reglen, textlen);
        return textlen;
    }

    return -1;
}

static inline int mln_get_char(unsigned int flag, char *s, int len)
{
    if (len <= 0) return 0;

    if (s[0] == '\\' && len > 1) {
        switch (s[1]) {
            case 'n':
                return '\n';
            case 't':
                return '\t';
            case '-':
                return '-';
            case '|':
                return '|';
            case '\\':
                return '\\';
            case 'a':
                return M_REGEXP_ALPHA;
            case 'd':
                return M_REGEXP_NUM;
            case 'D':
                return M_REGEXP_NOT_NUM;
            default:
                return s[1];
        }
    }

    switch (s[0]) {
        case '{':
            return M_REGEXP_LBRACE;
        case '}':
            return M_REGEXP_RBRACE;
        case '(':
            return M_REGEXP_LPAR;
        case ')':
            return M_REGEXP_RPAR;
        case '[':
            return M_REGEXP_LSQUAR;
        case ']':
            return M_REGEXP_RSQUAR;
        case '^':
            return M_REGEXP_XOR;
        case '*':
            return M_REGEXP_STAR;
        case '$':
            return M_REGEXP_DOLL;
        case '.':
            return M_REGEXP_DOT;
        case '?':
            return M_REGEXP_QUES;
        case '+':
            return M_REGEXP_PLUS;
        case '-':
            if (flag & M_REGEXP_MASK_SQUARE)
                return M_REGEXP_SUB;
            return '-';
        case '|':
            return M_REGEXP_OR;
        default:
            break;
    }

    return s[0];
}

static inline int mln_get_length(char *s, int len)
{
    if (s[0] == '\\' && len > 1) {
        switch (s[1]) {
            case 'n':
            case 't':
            case '-':
            case '|':
            case 'a':
            case 'd':
            case 'D':
            case '\\':
            default:
                return 2;
        }
    }

    return 1;
}

int mln_reg_match(mln_string_t *exp, mln_string_t *text)
{
    return mln_match_here(M_REGEXP_MASK_NEW, exp->str, text->str, exp->len, text->len);
}

int mln_reg_equal(mln_string_t *exp, mln_string_t *text)
{
    return !mln_match_here(M_REGEXP_MASK_NEW, exp->str, text->str, exp->len, text->len);
}
