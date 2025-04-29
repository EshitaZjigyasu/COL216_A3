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
};

class Core {
public:
    std::vector<Instruction> trace;
    int current_instr;
    int cycle_count;
    int idle_cycles;
    bool is_blocked;
    int number_of_writebacks;
    Cache* cache;
    bool done;
    RunResult result;
    int when_free;

    Core(std::vector<Instruction> trace, Cache* cache);
    void run();

    // this function is only called when the current core actually has the particular memory address in its cache (in one of the valid states)
};

#endif // CORE_HPP