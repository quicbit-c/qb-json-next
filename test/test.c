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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tap.h>
#include "next.h"

int fprint_tokens (FILE* dst, char* src, int src_len, int toks_per_line) {
  pstate* ps = new_ps(src, 0, src_len, 1000);
  char buf[2048];
  int len = 0;
  while (next(ps, NULL)) {
    tokstr(buf, ps, 0);
    len += fprintf(dst, "%s\n", buf);
  }
  return len;
}

int main () {
  FILE * fp;

  char* fname = "test/blockchain_unconfirmed.json";
  fp = fopen(fname, "rb");
  if (fp == NULL) { fprintf(stderr, "error opening '%s'\n", fname); exit(1); }

  // calculate file size:
  fseek(fp , 0 , SEEK_END);
  int src_len = ftell(fp);
  rewind(fp);

  char* src = (char*) malloc(src_len);
  if (src == NULL) { fprintf(stderr, "memory error\n"); exit(2); }
  if (fread(src, 1, src_len, fp) != src_len) { fprintf(stderr, "read error\n"); exit(3); }

  // fprintf(stdout, "read %d bytes from '%s'\n", src_len, fname);

  int toks_per_line = 16;
  fprint_tokens(stdout, src, src_len, toks_per_line);

  fclose(fp);
  return 0;
}