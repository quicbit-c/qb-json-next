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

