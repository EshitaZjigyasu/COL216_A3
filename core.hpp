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

#include <vector>
#include "cache.hpp"

struct Instruction {
    bool is_read;
    int address;
};

class Core {
public:
    std::vector<Instruction> trace;
    int current_instr;
    int cycle_count;
    int idle_cycles;
    bool is_blocked;
    Cache* cache;
    bool done;

    Core(std::vector<Instruction> trace, Cache* cache) {
        this->trace = trace;
        this->current_instr = 0;
        this->cycle_count = 0;
        this->idle_cycles = 0;
        this->is_blocked = false;
        this->cache = cache;
        this->done = false;
    }
};