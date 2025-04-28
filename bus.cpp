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

    Bus() {
        this->bytes_transferred = 0;
        this->invalidations = 0;
    }

    bool request_bus(BusMessage message) {
        if (is_free) {
            this->message = message;
            is_free = false;
            return true;
        }
        return false;
    }
};