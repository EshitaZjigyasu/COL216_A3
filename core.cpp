#include <vector>
#include "core.hpp"
#include <iostream>
using namespace std;

// class Core {
// public:
    // std::vector<Instruction> trace;
    // int current_instr;
    // int cycle_count;
    // int idle_cycles;
    // bool is_blocked;
    // int number_of_writebacks;
    // Cache* cache;
    // bool done;
    // int when_free;
    // RunResult result;

    Core::Core(std::vector<Instruction> trace, Cache* cache) {
        // cout << "Core constructor called" << endl;
        this->trace = trace;
        this->current_instr = 0;
        // this->cycle_count = 0;
        this->idle_cycles = 0;
        this->is_blocked = false;
        this->cache = cache;
        // cout << "hi im hereeeee " << this->cache->block_size << endl;
        this->done = false;
        result.set_index = -1;
        result.tag = -1;
        result.is_read = true;
        result.state = INVALID;
        result.hit = false;
        result.done = false;
        // cout << "Core constructor finished" << endl;
    }

    //run returns true if cache hit, false if cache miss
    void Core::run() {
        Instruction instr;
        if(is_blocked && (when_free == -1)) {
            return;
        } else if(is_blocked && (when_free != -1)) {
            return;
        } else {
            if(current_instr < trace.size()) {
                instr = trace[current_instr++];
            }
            int address = instr.address;
            bool is_read = instr.is_read;
            
            int set_index = (address / block_size) % no_of_sets;
            int tag = address / (block_size * no_of_sets);

            if (current_instr > trace.size()) {
                this->result.done = true;
                this->total_execution_cycles = cycle_count;
                cout << cycle_count << endl;
                return;
            }

            if (is_read) {
                this->number_of_reads++;
            } else {
                this->number_of_writes++;
            }

            // cout << "Core " << this << " running instruction: " << (is_read ? "READ" : "WRITE") << " at address " << address << endl;
            CacheLine* line = cache->access(set_index, tag, cycle_count);
            // cout << "Core " << this << " accessing cache line with tag " << tag << " at set index " << set_index << endl;
            
            this->result.done = false;
            this->result.set_index = set_index;
            this->result.tag = tag;
            this->result.is_read = is_read;
            this->result.state = line->state;
            this->result.hit = !line->evicted;
            this->result.line = line;
        }

        // Instruction instr = trace[current_instr++];
        
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
// };