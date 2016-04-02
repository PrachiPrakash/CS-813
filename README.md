##This containst the following two hacks
###1. Allocator
this contains a naive implrmrntation of malloc which uses a 16k array to store both the metadata and the data of a allocator it can be easily change to accomodate the sbrk(sbreak) system call.

###2.Pattern
This contains a hack which write any arbitary bit pattern into the bytes specified by the start address andthe end address,and also chaeck till what point a particular bitstring matches from the start to end address.
