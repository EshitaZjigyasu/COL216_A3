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

    Processor(vector<Instruction>** traces, int number_of_sets, int associativity, int block_size);

    void simulate();


};