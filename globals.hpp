enum State {
    MODIFIED,
    EXCLUSIVE,
    SHARED,
    INVALID
};

int cycle_count; 
int no_of_sets;
int no_of_blocks;
int block_size;