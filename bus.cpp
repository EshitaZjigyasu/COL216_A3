#include "bus.hpp"

enum BusOperation {
    MEMREAD,
    RWITM,
    INVALIDATE,
    C_TO_C,
    CTOC_THEN_WRITE, // occurs when read miss, and another core has the block in modified state
    WRITE_THEN_READ, // occurs when write miss, and another core has the block in modified state
    EVICT_THEN_MEMREAD, // occurs when read miss, and need to evict a block first
    EVICT_THEN_RWITM, // occurs when write miss, and need to evict a block first
};

struct BusMessage {
    BusOperation operation;
    int index;
    int tag;
    int local_core_id;
    int remote_core_id;
    bool is_read;
    int set_local; // used for CTOC_THEN_WRITE, the time at which local needs to be set to S 
    int set_remote; // used for WRITE_THEN_READ, the time at which remove needs to be set to I
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
            // else if(message.operation == CTOC_THEN_WRITE) {
            //     //TODO
            //     // when_free = cycle_count + 2N + 100;
            //     // set_local = cycle_count + 2N; (message.set_local)
            //     // change_state --> true for local and remote
            //     // change to --> SHARED for local and SHARED for remote
            // }
            // else if(message.operation == WRITE_THEN_READ) {
            //     //TODO
            //     // when_free = cycle_count + 200;
            //     // set_remote = cycle_count + 100; (message.set_remote)
            //     // change_state --> true for local and remote
            //     // change to --> INVALID for remote and MODIFIED for local
            // }
            else if(message.operation == EVICT_THEN_MEMREAD || message.operation == EVICT_THEN_RWITM) {
                when_free = cycle_count + 200;
            } else {
                when_free = cycle_count + 100;
            }
            return true;
        }
        return false;
    }

    void take_control();
};

