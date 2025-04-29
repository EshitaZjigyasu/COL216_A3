#ifndef GLOBALS_HPP
#define GLOBALS_HPP

enum State {
    MODIFIED,
    EXCLUSIVE,
    SHARED,
    INVALID
};

extern int cycle_count; 
extern int no_of_sets;
extern int no_of_blocks;
extern int block_size;

#endif // GLOBALS_HPP