#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "core.hpp"
#include <vector>
using namespace std;

class Processor {
public:

    // Core** cores;
    // Cache** caches;
    vector<Core*> cores;
    vector<Cache*> caches;
    Bus* bus;

    Processor(vector<Instruction>(&traces)[4], int number_of_sets, int associativity, int block_size);

    ~Processor();

    void simulate();


};

#endif // PROCESSOR_HPP