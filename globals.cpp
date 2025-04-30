#include "globals.hpp"

long long cycle_count = 0;
int no_of_sets = 64; 
int no_of_blocks = 2;
int block_size = 32;

CacheLine::CacheLine(int block_size) {
    this->tag = 0;
    this->state = INVALID;
    this->block_size = block_size;
}