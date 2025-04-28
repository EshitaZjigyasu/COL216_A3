#include "globals.hpp"

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

    bool change_state[4];
    State change_to[4];

    Bus();

    bool request_bus(BusMessage message);
};