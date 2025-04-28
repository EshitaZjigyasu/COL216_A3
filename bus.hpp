class Bus {
    public:
        int invalidations;
        int traffic;

        Bus_signal cur_signal;
        int cur_addr;
        int cur_core_id;
};

enum Bus_signal {
    INVALIDATE,
    RWITM,
    MEM_READ
};