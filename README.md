# COL216_A3

## How to use tha makefile
make --> makes an executable named L1simulate
make run ARGS="<<args to be given to the executable>> --> to run the executable with the arguments given inside the double quotes

## Custom test cases -
1. cache to cache transfer
core1 takes a total of 166 cycles to finish since it takes the data from core0, instead of taking 202 cycles and taking data from the main memory.
2. read miss in core1, core0 has the value in modified state
Execution takes a total of 469 cycles. First, it waits for core0 to complete its MEMREAD. Then, it has two read misses which takes another 101 cycles each. Then, it takes the value from core0 (cache to cache transfer - 2*N cycles), and core0 then write backs to memory. 
3. miss after address crosses block size 
There are only 2 misses if the addresses in traces are consecutive (36 consecutive addresses), this shows the block size of the cache.
4. write miss in core1, core0 has the value in modified state
Execution takes a total of 404 cycles. 
5. read miss in core1, core0 has the value. then core0 writes to the same address causing the line in core1 to be invalidated leading to another miss in core1.
6. write miss in core1, core0 has the value. write miss leads to taking value from memory even though another cache has it. 
Hence it takes 202 cycles (first 101 for read miss in core0, next 100 cycles to read from memory and 1 cycle to invalidate)
7. eviction in core0 at 3rd instruction. hence core1 can't do cache-to-cache transfer and has to take line from memory