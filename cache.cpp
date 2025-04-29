#include <vector>
#include "globals.hpp"

class CacheLine {
public:
    int tag;
    State state;
    int block_size;
    int last_access_time;
    bool evicted;

    CacheLine(int block_size) {
        this->tag = 0;
        this->state = INVALID;
        this->block_size = block_size;
    }
};

class Set {
public:
    std::vector<CacheLine> lines;
    int number_of_lines;
    

    Set(int associativity, int block_size) {
        for (int i = 0; i < associativity; ++i) {
            lines.push_back(CacheLine(block_size));
        }
        this->number_of_lines = associativity;
    }

    CacheLine* find_line(int tag) {
        // returns pointer to line if found, else returns nullptr
        for (auto& line : lines) {
            if (line.tag == tag) {
                return &line;
            }
        }
        return nullptr;
    }

    CacheLine* line_to_replace() {
        int min_time = -1;
        CacheLine* line_to_replace = nullptr;
        for(int i = 0; i < number_of_lines; i++){
            if (lines[i].state == INVALID) {
                return &lines[i];
            }
            if (min_time == -1 || lines[i].last_access_time < min_time) {
                min_time = lines[i].last_access_time;
                line_to_replace = &lines[i];
            }
        }
        return line_to_replace;
    }
};

class Cache {
public:
    std::vector<Set> sets;
    int number_of_sets;
    int number_of_lines;
    int number_of_evictions;
    int number_of_misses;
    int block_size;

    Cache(int number_of_sets, int associativity, int block_size) {
        this->number_of_sets = number_of_sets;
        this->number_of_lines = associativity;
        this->block_size = block_size;

        for (int i = 0; i < number_of_sets; ++i) {
            sets.push_back(Set(associativity, block_size));
        }
    }

    CacheLine* access(int set_index, int tag, int current_time) {
        // if hit -> returns actual line, with evicted = false
        // if miss -> returns line to replace, with evicted = true
        Set& set = sets[set_index];
        CacheLine* line = set.find_line(tag);
        if (line) {
            line->evicted = false;
            line->last_access_time = current_time;
            return line;
        } else {
            this->number_of_misses++;
            CacheLine* line_to_replace = set.line_to_replace();
            line_to_replace->evicted = true;
            if (line_to_replace->state != INVALID) {
                // TODO: check if eviction means writeback / setting invalid / replacing with some other address
                // what happens if you replace an invalid line? is it an eviction? if not then is setting the line to invalid considered an eviction?
                this->number_of_evictions++;
            }
            return line_to_replace;
        }
    }



};