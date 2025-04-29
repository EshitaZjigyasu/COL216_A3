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
    int when_free;
    RunResult result;

    Core(std::vector<Instruction> trace, Cache* cache) {
        this->trace = trace;
        this->current_instr = 0;
        this->cycle_count = 0;
        this->idle_cycles = 0;
        this->is_blocked = false;
        this->cache = cache;
        this->done = false;
        result = RunResult {
            -1,
            -1,
            true,
            State::INVALID,
            false,
            false
        };
    }

    //run returns true if cache hit, false if cache miss
    void run() {
        Instruction instr = trace[current_instr++];
        int address = instr.address;
        bool is_read = instr.is_read;

        int set_index = (address / cache->block_size) % cache->number_of_sets;
        int tag = address / (cache->block_size * cache->number_of_sets);

        if (current_instr >= trace.size()) {
            this->result.done = true;
            return;
        }

        CacheLine* line = cache->access(set_index, tag, cycle_count);
        
        this->result.done = false;
        this->result.set_index = set_index;
        this->result.tag = tag;
        this->result.is_read = is_read;
        this->result.state = line->state;
        this->result.hit = !line->evicted;
    }

    // State snoop(BusMessage message) {
    //     CacheLine* line = cache->sets[message.index].find_line(message.tag);
    //     if (line->evicted) {
    //         return State::INVALID;
    //     }
    //     if (line->state == MODIFIED) {
    //         return State::MODIFIED;
    //     } else if (line->state == EXCLUSIVE) {
    //         return State::EXCLUSIVE;
    //     } else if (line->state == SHARED) {
    //         return State::SHARED;
    //     }
    // }

    // this function is only called when the current core actually has the particular memory address in its cache (in one of the valid states)
    // void update_state(int tag, int set_index, State state) {
    //     CacheLine* line = cache->access(set_index, tag, cycle_count);
    //     line->state = state;
    // }
};