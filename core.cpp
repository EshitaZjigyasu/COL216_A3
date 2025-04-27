#include <vector>
#include <string>
#include "core.hpp"
using namespace std;

class Core {
public:
    

    int cur_instr;
    bool blocked;
    bool bus_access;
    bool done;
    int reads;
    int writes;
    int execution_cycles;
    int idle_cycles;
    int when_free;
    Cache* cache;
    vector<Instr>* instr_list;

    Core(Cache* cache, vector<Instr> *instr_list) {
        this->cache = cache;
        this->instr_list = instr_list;
        cur_instr = -1;
        blocked = false;
        bus_access = false;
        done = false;
        reads = 0;
        writes = 0;
        execution_cycles = 0;
        idle_cycles = 0;
        when_free = 0;
    }

    // int run_single_cycle() {
    //     if (!blocked) {
    //         if (cur_instr < instr_list->size() - 1) {
    //             execution_cycles++;
    //             cur_instr++;
    //             if ((instr_list[cur_instr]).read) {
    //                 reads++;
    //             } else {
    //                 writes++;
    //             }
    //             return instr_list[cur_instr].memory_addr;
    //         }
    //         if (cur_instr == instr_list.size() - 1) {
    //             done = true;
    //             return -1;
    //         }
    //     }
    //     else {
    //         idle_cycles++;
    //     }
    //     return instr_list[cur_instr].memory_addr;
    // }

    bool snoop(int requested_mem_addr) {
        
    }
};