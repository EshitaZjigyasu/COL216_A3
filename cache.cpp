#include <vector>
#include "cache.hpp"
#include <iostream>
using namespace std;

CacheLine::CacheLine(int block_size) {
    this->tag = 0;
    this->state = INVALID;
    this->block_size = block_size;
}

// CacheLine::~CacheLine() {
//     cout << "CacheLine destructor called" << endl;
// }

Set::Set(int associativity, int block_size) {
    for (int i = 0; i < associativity; ++i) {
        CacheLine* line = new CacheLine(block_size);
        lines.push_back(line);
    }
    this->number_of_lines = associativity;
    // cout << "Set initialized with " << lines.size() << " lines" << endl;
    // std::cout << "Set this pointer: " << this << ", number_of_lines set to " << number_of_lines << std::endl;

}
// Set::Set(int associativity, int block_size)
// : number_of_lines(associativity)
// {
//     lines.reserve(associativity);
//     for (int i = 0; i < associativity; ++i) {
//         lines.emplace_back(block_size);
//     }
//     std::cout << "Set initialized with " << lines.size() << " lines\n";
// }

Set::~Set() {
    cout << "Set destructor called" << endl;
    for (int i = 0; i < number_of_lines; ++i) {
        delete lines[i];
    }
    // cout << "Set destructor finished" << endl;
}

// Set::Set(Set&& other) noexcept {
//     cout << "Set move constructor called" << endl;
//     this->lines = std::move(other.lines); // Transfer ownership of the vector
//     this->number_of_lines = other.number_of_lines;

//     // Reset the source object to a valid state
//     other.number_of_lines = 0;
// }

CacheLine* Set::find_line(int tag) {
    // returns pointer to line if found, else returns nullptr

    // for (auto& line : lines) {
    //     cout << "Checking line with tag " << line.tag << endl;
    //     if (line.tag == tag) {
    //         cout << "Found line with tag " << tag << endl;
    //         return &line;
    //     }
    // }
    for (int i = 0; i < number_of_lines; i++) {
        // cout << "Checking line with tag " << (*lines[i]).tag << endl;
        CacheLine l = *lines[i];
        int t = l.tag;
        if (t == tag) {
            // cout << "Found line with tag " << tag << endl;
            return lines[i];
        }
    }
    return nullptr;
}

CacheLine* Set::line_to_replace() {
    int min_time = -1;
    CacheLine* line_to_replace = nullptr;
    for(int i = 0; i < number_of_lines; i++){
        if ((*lines[i]).state == INVALID) {
            return lines[i];
        }
        if (min_time == -1 || (*lines[i]).last_access_time < min_time) {
            min_time = (*lines[i]).last_access_time;
            line_to_replace = lines[i];
        }
    }
    return line_to_replace;
}

Cache::Cache(int number_of_sets, int associativity, int block_size) {
    // cout << "Cache constructor called" << endl;
    this->number_of_sets = number_of_sets;
    this->number_of_lines = associativity;
    this->block_size = block_size;

    for (int i = 0; i < number_of_sets; ++i) {
        Set* sett = new Set(associativity, block_size);
        sets.push_back(sett);
        // cout << sets.size() << endl;sq   
    }
    // cout << "Cache constructor finished" << endl;
}

CacheLine* Cache::access(int set_index, int tag, int current_time) {
    // if hit -> returns actual line, with evicted = false
    // if miss -> returns line to replace, with evicted = true
    Set* set = sets[set_index];
    CacheLine* line = set->find_line(tag);
    // cout << "Cache accessing line with tag " << tag << " at set index " << set_index << endl;
    if (line) {
        line->evicted = false;
        line->last_access_time = current_time;
        return line;
    } else {
        this->number_of_misses++;
        CacheLine* line_to_replace = set->line_to_replace();
        line_to_replace->evicted = true;
        // if (line_to_replace->state != INVALID) {
        //     // TODO: check if eviction means writeback / setting invalid / replacing with some other address
        //     // what happens if you replace an invalid line? is it an eviction? if not then is setting the line to invalid considered an eviction?
        //     this->number_of_evictions++;
        // }
        if (line_to_replace->state == MODIFIED) {
            this->number_of_evictions++;
            this->number_of_writebacks++;
        } else if (line_to_replace->state == SHARED || line_to_replace->state == EXCLUSIVE) {
            this->number_of_evictions++;
        }
        return line_to_replace;
    }
}
