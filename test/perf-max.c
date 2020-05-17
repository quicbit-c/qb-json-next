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
#include "next.h"

const int TOK_BUF_LIM = 100;
int main (int argc, char **argv) {
  FILE * fp;
  int flen;
  char * buf;

  char* fname = "../../../dev/json-samples/cache_150mb.json";
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
  double size_mb = (double)flen / (1024 * 1024);
  int iter = 5;
  int time_ticks[iter];
  fprintf(stdout, "read %f MB from '%s'\n", size_mb, fname);
  for (int i=0; i<iter; i++) {
      clock_t t0 = clock();
      for (int j=0; j<flen; j++) {
        if(buf[j] == 0) {
            fprintf(stdout, "ERROR at byte %d\n", j);
            return -1;
        }
      }
      clock_t t1 = clock();
      time_ticks[i] = t1 - t0;
      rewind(fp);
  }
  fprintf(stdout, "done\n");
  double tot_seconds = 0; 
  for (int i=0; i<iter; i++) {
    double seconds = ((double)time_ticks[i])/CLOCKS_PER_SEC;
    fprintf(stdout, "Pass %d: %f seconds\n", i+1, seconds);
    tot_seconds += seconds;
  }

  fprintf(stdout, "%f MB per second\n", (size_mb * iter)/tot_seconds);
  
  fclose(fp);
  free(buf);
  return 0;
}

// Results on Mid-2014 Macbook Pro
//
// dads-MBP:qb-json-next dad$ ./test/perf-max
// read 144.333522 MB from '../../../dev/json-samples/cache_150mb.json'
// done
// Pass 1: 0.076203 seconds
// Pass 2: 0.070643 seconds
// Pass 3: 0.070570 seconds
// Pass 4: 0.070286 seconds
// Pass 5: 0.071208 seconds
// 2010.720262 MB per second

