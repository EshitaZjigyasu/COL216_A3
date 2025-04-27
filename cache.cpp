#include <vector>
#include <string>
#include "cache.hpp"
using namespace std;

class Cache {
    void initializeCache() {
        cache = vector<Set>(no_of_sets, Set{vector<Block>(no_of_blocks, Block{INVALID, -1, -1})});
    }

    public:
        int misses = 0;
        int evictions = 0;
        int write_backs = 0;
        int no_of_sets;
        int no_of_blocks;
        int block_size;
        vector<Set> cache;
        
        Cache(int b, int l, int bs) {
            no_of_sets = b;
            no_of_blocks = l;
            block_size = bs;
            initializeCache();
        }
        bool read(int mem_addr);
        bool write(int mem_addr);
};