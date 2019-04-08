// Software License Agreement (ISC License)
//
// Copyright (c) 2019, Matthew Voss
//
// Permission to use, copy, modify, and/or distribute this software for
// any purpose with or without fee is hereby granted, provided that the
// above copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef NEXT_H
#define NEXT_H

// values for ps.pos(ition).  LSB (0x7F) are reserved for token ascii value.
static const int POS_A_BF = 0x080;   // in array, before first value
static const int POS_A_BV = 0x100;   // in array, before value
static const int POS_A_AV = 0x180;   // in array, after value
static const int POS_O_BF = 0x200;   // in object, before first key
static const int POS_O_BK = 0x280;   // in object, before key
static const int POS_O_AK = 0x300;   // in object, after key
static const int POS_O_BV = 0x380;   // in object, before value
static const int POS_O_AV = 0x400;   // in object, after value

//Object.keys(POS).reduce(function (a, n) { a[POS[n]] = n; return a }, [])

// values for ps.tok(en).  All but string and decimal are represented by the first ascii byte encountered
static const int TOK_ARR = 91;        // [    array start
static const int TOK_ARR_END = 93;    // ]    array end
static const int TOK_DEC = 100;       // d    a decimal value starting with = -, 0, 1, ..., 9
static const int TOK_FAL = 102;       // f    false
static const int TOK_NUL = 110;       // n    null
static const int TOK_STR = 115;       // s    a string value starting with "
static const int TOK_TRU = 116;       // t    true
static const int TOK_OBJ = 123;       // {    object start
static const int TOK_OBJ_END = 125;   // }    object end

// for an unexpected or illegal value, or if src limit is reached before a value is complete, ps.tok will be zero
// and ps.ecode will be one of the following:
static const int ECODE_BAD_VALUE = 66;    // 'B'  encountered invalid byte or series of bytes
static const int ECODE_TRUNC_DEC = 68;    // 'D'  end of buffer was a decimal ending with a digit (0-9). it is *possibly* unfinished
static const int ECODE_KEY_NO_VAL = 75;   // 'K'  object key complete, but value did not start
static const int ECODE_TRUNCATED = 84;    // 'T'  key or value was unfinished at end of buffer
static const int ECODE_UNEXPECTED = 85;   // 'U'  encountered a recognized token in wrong place/context

typedef struct ps {
    int soff;
    char* src;
    int src_len;

    int lim;
    int koff;
    int klim;
    int voff;
    int vlim;
    int tok;
    int* stack;
    int stack_len;
    int pos;
    int ecode;
    int vcount;
    int line;
    int lineoff;

    char* next_src;
    int next_src_len;
} pstate;

pstate* new_ps (char* src, int off, int lim, int max_depth);

typedef struct nopt {
    int (*err)(pstate*);
} next_opt;

pstate init (pstate ps);
int next (pstate* ps, next_opt* opt);
char* pos_name (int pos);
void print_ps (pstate* ps);
int tokstr (char* buf, int lim, pstate* ps, int detail);

#endif      // NEXT_H