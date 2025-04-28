#include "core.hpp"
#include "cache.hpp"
#include "bus.hpp"
#include <vector>
using namespace std;

class Processor {
public:
    Core* core0;
    Core* core1;
    Core* core2;
    Core* core3;
    Cache* cache0;
    Cache* cache1;
    Cache* cache2;
    Cache* cache3;
    Bus* bus;

    Processor(std::vector<Instruction>* trace0, std::vector<Instruction>* trace1, std::vector<Instruction>* trace2, std::vector<Instruction>* trace3, int number_of_sets, int associativity, int block_size) {
        this->cache0 = &Cache(number_of_sets, associativity, block_size);
        this->cache1 = &Cache(number_of_sets, associativity, block_size);
        this->cache2 = &Cache(number_of_sets, associativity, block_size);
        this->cache3 = &Cache(number_of_sets, associativity, block_size);
        this->core0 = &Core(trace0, cache0);
        this->core1 = &Core(trace1, cache1);
        this->core2 = &Core(trace2, cache2);
        this->core3 = &Core(trace3, cache3);
        this->bus = &Bus();
    }

    void simulate() {
        while (true) {
            if (core0->done && core1->done && core2->done && core3->done) {
                break;
            }
            if (core0->is_blocked || core1->is_blocked || core2->is_blocked || core3->is_blocked) {
                bus->when_free++;
                continue;
            }
            if (core0->current_instr < core0->trace.size()) {
                core0->run();
            }
            if (core1->current_instr < core1->trace.size()) {
                core1->run();
            }
            if (core2->current_instr < core2->trace.size()) {
                core2->run();
            }
            if (core3->current_instr < core3->trace.size()) {
                core3->run();
            }
        }
    }


};