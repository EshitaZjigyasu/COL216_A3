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

#include <vector>

enum State {
    MODIFIED,
    EXCLUSIVE,
    SHARED,
    INVALID
};

class CacheLine {
public:
    int tag;
    int data;
    State state;
    bool valid;
    int block_size;
    int last_access_time;
    bool evicted;

    CacheLine(int block_size) {
        // Initialize cache line with default values
        this->tag = 0;
        this->data = 0;
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

    CacheLine* find_line(int tag, int current_time) {
        for (auto& line : lines) {
            if (line.tag == tag) {
                return &line;
                line.last_access_time = current_time;
            }
        }
        return nullptr;
    }

    CacheLine* line_to_replace() {
        int min_time = -1;
        CacheLine* line_to_replace = nullptr;
        for(int i = 0; i < number_of_lines; i++){
            if (!lines[i].valid) {
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
    int block_size;

    Cache(int number_of_sets, int associativity, int block_size) {
        this->number_of_sets = number_of_sets;
        this->number_of_lines = associativity;
        this->block_size = block_size;

        for (int i = 0; i < number_of_sets; ++i) {
            sets.push_back(Set(associativity, block_size));
        }
    }

    // CacheLine* access(int set_index, int tag, int current_time) {
    //     Set& set = sets[set_index];
    //     CacheLine* line = set.find_line(tag, current_time);
    //     if (line) {
    //         line->evicted = false;
    //         return line;
    //     } else {
    //         CacheLine* line_to_replace = set.line_to_replace();
    //         if (line_to_replace->valid) {
    //             line_to_replace->evicted = true;
    //         }
    //         return set.line_to_replace();
    //     }
    // }

    bool read_line(int set_index, int tag, int current_time) {
        Set& set = sets[set_index];
        CacheLine* line = set.find_line(tag, current_time);
        return line->valid; //if not valid, means it's in INVALID MESI state
    }

};