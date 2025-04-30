#ifndef GLOBALS_HPP
#define GLOBALS_HPP

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
    
        CacheLine(int block_size);
        // ~CacheLine();
    };

extern long long cycle_count; 
extern int no_of_sets;
extern int no_of_blocks;
extern int block_size;

#endif // GLOBALS_HPP