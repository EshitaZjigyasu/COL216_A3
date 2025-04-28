#include "bus.hpp"
using namespace std;

class Bus {
    public:
        int invalidations = 0;
        int traffic = 0;

        
        Bus_signal cur_signal;
        int cur_addr;
        int cur_core_id;
};