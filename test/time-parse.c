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

const int TOK_BUF_LIM = 100;
int main () {
  FILE * fp;
  int flen;
  char * buf;
  char tokbuf[TOK_BUF_LIM];

//  char* fname = "../../json-samples/cache_150mb.json";
  char* fname = "../../json-samples/cache_150mb.json";
  fp = fopen(fname, "rb");
  if (fp == NULL) {
    fprintf(stderr, "error opening '%s'\n", fname); exit(1);
  }

  // obtain file size:
  fseek(fp , 0 , SEEK_END);
  flen = ftell(fp);
  rewind(fp);

  buf = (char*) malloc(flen);
  if (buf == NULL) {
    fprintf(stderr, "memory error\n"); exit(2);
  }

  if (fread(buf, 1, flen, fp) != flen) {
    fprintf(stderr, "read error\n"); exit(3);
  }
  fprintf(stdout, "read %d bytes from '%s'\n", flen, fname);
  for (int i=0; i<5; i++) {
      clock_t t0 = clock();
      pstate* ps = new_ps(buf, 0, flen, 1000);
      while (next(ps, NULL)) {
//        print_ps(ps);
      }
      clock_t t1 = clock();
      fprintf(stdout, "parsing finished\n");
      fprintf(stdout, "%f seconds\n", ((double)(t1 - t0) / CLOCKS_PER_SEC));
      tokstr(tokbuf, TOK_BUF_LIM, ps, 0);
      printf("%s\n", tokbuf);
      rewind(fp);
  }
  fclose(fp);
  free(buf);
  return 0;
}
/*
int main(int argc, char **argv) {
    char* = fopen
    char* input = "{\"a\":7, \"b\":[7,8,999] }";
    pstate* ps = new_ps(input, 0, strlen(input), 100);
    char buf[TOK_BUF_LIM];
    while (next(ps, NULL)) {
//        print_ps(ps);
        tokstr(buf, TOK_BUF_LIM, ps, 0);
        printf("%s\n", buf);
    }

//	int rc = ok(1 == 1, "1 equals 1");
//    printf("rc: %d\n", rc);

    return 0;
}

*/

