#include "cache.hpp"
#include "instr.hpp"

class Core {
public:
    Cache cache;

    int cur_instr;
    bool blocked;
    bool bus_access;
    bool done;
    int reads;
    int writes;
    int execution_cycles;
    int idle_cycles;
    int when_free;
    vector<Instr> instr_list;

    Core(Cache* cache, vector<Instr> *instr_list);

    int run_single_cycle();

    bool snoop(int requested_mem_addr);
};