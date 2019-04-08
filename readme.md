# qb-json-next

This is the C version of [qb-json-next](https://github.com/quicbit-js/qb-json-next) json parser that was first 
made available in javascript.  It too is a zero-dependency light-weight library that parses json at a 
rate of 800 MB/second.


## Optimizations

For the curious, here is a summary of optimization experience working with a 150 MB json file (the node js
index of projects) on my 2.2GHz i7 quad core laptop (macos):  

Compiling **without optimization** flags and without parallelization, next parses at **230 MB per second**, which
is slower than the nodejs version.  

With optimization set to -O3, performance jumps to **550 MB per second**. 

With optimization set and unfolding of string and decimal loops (allowing for some load parallelization),
performance increases to **800 MB per second**.

With some more optimization, next.c could likely exceed 1 GB per second parsing. 

## API

API is much the same as the javascript API, but with the C library, instead of using any struct/object,
callers must first create the parse state structure "pstate" by calling new_ps():

### pstate* new_ps (char* src, int off, int lim, int max_depth)

* src: the source to be parsed
* off: the offset from where to start parsing
* lim: the limit (non-inclusive) where parsing should end
* max_depth: the maximum nesting depth to handle (max depth for object within array within object...)

    pstate* ps = new_ps(buf, 0, buf_length, 1000);




### next (pstate* ps)

next() works just like the javascript version, except options is currently just a placeholder (unused). So
call it with NULL:

    pstate* ps = new_ps (buffer, 0, buffer_length, 1000);
    next(ps, NULL);
    
    // every call to next() will update the parse state.
    
The javascript version of qb-json-next **[has more documentation on parse state](https://github.com/quicbit-js/qb-json-next#the-parse-state-object-ps).**
Just substitute "char*" for the \[byt\] types.
 