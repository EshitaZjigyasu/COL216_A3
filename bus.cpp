#include "bus.hpp"

enum BusOperation {
    MEMREAD,
    RWITM,
    INVALIDATE,
};

struct BusMessage {
    BusOperation operation;
    int index;
    int tag;
    int core_id;
    bool is_read;
};

class Bus {
public:
    int bytes_transferred;
    int invalidations;
    BusMessage message;
    int when_free;
    bool is_free;

    bool change_state[4] = {false, false, false, false};
    State change_to[4] = {INVALID, INVALID, INVALID, INVALID};

    Bus() {
        this->bytes_transferred = 0;
        this->invalidations = 0;
    }

    bool request_bus(BusMessage message) {
        if (is_free) {
            this->message = message;
            is_free = false;
            bytes_transferred += block_size;
            if(message.operation == INVALIDATE) {
                when_free = cycle_count + 1;
            }
            else {
                when_free = cycle_count + 100;
            }
            return true;
        }
        return false;
    }

    void take_control();
};

