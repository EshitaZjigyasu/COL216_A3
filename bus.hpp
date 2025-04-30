#ifndef BUS_HPP
#define BUS_HPP

#include "globals.hpp"

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
    State state_to_evict;
    CacheLine* local_line;
    bool change_local;
    State change_local_to;
};

class Bus {
public:
    int bytes_transferred;
    int invalidations;
    BusMessage message;
    int when_free;
    bool is_free;

    bool change_state[4];
    State change_to[4];

    Bus();

    bool request_bus(BusMessage message);
};

#endif // BUS_HPP