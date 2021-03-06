# qb-json-next

Three features set this JSON parser apart. qb-json-next is

1. Fast/Efficient:  Parses 800 MB/second on a 2.2 GHz macbook.  Parsing has no overhead regardless of file size.
2. Tiny with no dependencies. (about 350 lines of code in one file, library size is around 8 kb).
3. Incremental.  The parsing can start and stop and continue again at any point, even 
   between bytes in a unicode character which allows parsing across byte blocks.


This is the C version of [javascript qb-json-next](https://github.com/quicbit-js/qb-json-next) 
parser with optimizations that allow it to run roughly 2.5 times faster.

Note that unlike the javascript version, this C version of qb-json-next is beta (version 0.0.1).  While 
it successfully parsed large complex files, it does not yet comply 
with the [qb-standard 100% test coverage requirement](https://github.com/quicbit-js/qb-standard).

## Optimizations

For the curious, here is a summary of optimization experience working with a 150 MB json file (the node js
index of projects) on my 2.2GHz i7 quad core laptop (macbook pro Mid-2014):  

Compiling **without optimization** flags and without parallelization, next parses at **230 MB per second**, which
is slower than the nodejs version.  

With optimization set to -O3, performance jumps to **550 MB per second**. 

With optimization set and unfolding of string and decimal loops (allow parallel load handling across
cores), performance increases to **800 MB per second**.

The theoretical maximum performance based on number of bytes able to scan is **2010 MB per second** 
(results of [perf-max.c](https://github.com/quicbit-c/qb-json-next/blob/master/test/perf-max.c))

I suspect with some more optimization, qb-json-next could exceed 1 GB per second on a similar machine. 

## API

API is much the same as the javascript API, but with the C library, instead of using any struct/object,
callers must first create the parse state structure "pstate" by calling new_ps():

### pstate* new_ps (char* src, int off, int lim, int max_depth)

* src: the source to be parsed
* off: the offset from where to start parsing
* lim: the limit (non-inclusive) where parsing should end
* max_depth: the maximum nesting depth to handle (max depth for object within array within object...)

    pstate* ps = new_ps(buf, 0, buf_length, 1000);




### next (pstate* ps, next_opt* opt)

next() works just like the javascript version, except options is currently just a placeholder (unused). So
call it with NULL:

    pstate* ps = new_ps (buffer, 0, buffer_length, 1000);
    next(ps, NULL);
    
    // every call to next() will update the parse state.
    
The javascript version of qb-json-next **[has more documentation on parse state](https://github.com/quicbit-js/qb-json-next#the-parse-state-object-ps).**
Just substitute "char*" for the \[byt\] types.
 