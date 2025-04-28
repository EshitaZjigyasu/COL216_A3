enum BusOperation {
    MEMREAD,
    RWITM,
    INVALIDATE,
};

struct BusMessage {
    BusOperation operation;
    int address;
    int core_id;
};

class Bus {
public:
    int bytes_transferred;
    int invalidations;
    BusMessage message;
    int when_free;
    bool is_free;

    Bus();

    bool request_bus(BusMessage message);
};