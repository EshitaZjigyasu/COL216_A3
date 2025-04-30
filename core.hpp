// #include "cache.hpp"

// class Core {
//     Cache cache;
//     // instruction list
//     // current instruction
//     // free at which cycle
//     // flag for blocked or not
//     // flag for bus access

//     /*methods - 1. run
//     */

// }
#ifndef CORE_HPP
#define CORE_HPP

#include <vector>
#include "cache.hpp"

struct Instruction {
    bool is_read;
    int address;
};

struct RunResult {
    int set_index;
    int tag;
    bool is_read;
    State state;
    bool hit;
    bool done;
    CacheLine* line; // this is the line being written on / evicted
};

class Core {
public:
    std::vector<Instruction> trace;
    int current_instr;
    // int cycle_count;
    int idle_cycles;
    bool is_blocked;
    Cache* cache;
    bool done;
    RunResult result;
    int when_free;
    BusMessage bus_message;
    // BusMessage invalidate_message;
    int number_of_reads;
    int number_of_writes;
    int total_execution_cycles;
    int number_of_writebacks;
    int traffic;
    int bus_invalidations;

    Core(std::vector<Instruction> trace, Cache* cache);
    void run();

    // this function is only called when the current core actually has the particular memory address in its cache (in one of the valid states)
};

#endif // CORE_HPP