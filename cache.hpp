class Cache {
    void initializeCache();

    public:
        int misses;
        int evictions;
        int write_backs;
        int no_of_sets;
        int no_of_blocks;
        int block_size;
        
        Cache(int, int, int);
        bool read(int mem_addr);
        bool write(int mem_addr);
        vector<Set> cache;
};

enum State {
    INVALID,
    EXCLUSIVE,
    MODIFIED,
    SHARED
};

struct Block {
    State state;
    int block_size;
    int tag;
    int last_access_time;
};

struct Set {
    vector<Block> blocks;
};