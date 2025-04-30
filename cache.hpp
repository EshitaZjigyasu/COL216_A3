// class Cache {
// public:
//     Cache(int size, int blockSize) : size(size), blockSize(blockSize) {
//         numBlocks = size / blockSize;
//         cacheData.resize(numBlocks);
//         for (int i = 0; i < numBlocks; ++i) {
//             cacheData[i].resize(blockSize);
//         }
//     }

//     void read(int address) {
//         // Implement read logic
//     }

//     void write(int address, int data) {
//         // Implement write logic
//     }
// }
#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include "bus.hpp"

// class CacheLine {
// public:
//     int tag;
//     int data;
//     State state;
//     bool valid;
//     int block_size;
//     int last_access_time;
//     bool evicted;

//     CacheLine(int block_size);
//     // ~CacheLine();
// };



class Set {
public:
    std::vector<CacheLine*> lines;
    int number_of_lines;
    

    Set(int associativity, int block_size);

    ~Set();

    // Set(Set&& other) noexcept;

    CacheLine* find_line(int tag);

    CacheLine* line_to_replace();
};

class Cache {
public:
    std::vector<Set*> sets;
    int number_of_sets;
    int number_of_lines;
    int block_size;
    int number_of_evictions;
    int number_of_misses;
    int number_of_writebacks;

    Cache(int number_of_sets, int associativity, int block_size);

    CacheLine* access(int set_index, int tag, int current_time);

    // bool read_line(int set_index, int tag, int current_time) {
    //     Set& set = sets[set_index];
    //     CacheLine* line = set.find_line(tag, current_time);
    //     return line->valid; //if not valid, means it's in INVALID MESI state
    // }

};

#endif // CACHE_HPP