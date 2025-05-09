#include "bus.hpp"
#include <iostream>
using namespace std;

// class Bus {
// public:
    // int bytes_transferred;
    // int invalidations;
    // BusMessage message;
    // int when_free;
    // bool is_free;

    // bool change_state[4] = {false, false, false, false};
    // State change_to[4] = {INVALID, INVALID, INVALID, INVALID};

    Bus::Bus() {
        // cout << "Bus constructor called" << endl;
        this->bytes_transferred = 0;
        this->invalidations = 0;
        this->is_free = true;
        for(int i = 0; i < 4; i++) {
            change_state[i] = false;
            change_to[i] = INVALID;
        }
        this->message.state_to_evict = INVALID;
        // cout << "Bus constructor finished" << endl;
    }

    bool Bus::request_bus(BusMessage message) {
        if (is_free) {
            this->bus_transactions++;
            this->message = message;
            is_free = false;
            if(message.operation == INVALIDATE) {
                when_free = cycle_count + 1;
                this->invalidations++;
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
                if (message.state_to_evict == MODIFIED) {
                    when_free = cycle_count + 201;
                    bytes_transferred += block_size;
                } else if (message.operation == EVICT_THEN_MEMREAD) {
                    when_free = cycle_count + 101;
                    this->message.operation = MEMREAD;
                } else if (message.operation == EVICT_THEN_RWITM) {
                    when_free = cycle_count + 101;
                    this->message.operation = RWITM;
                }
            } else {
                when_free = cycle_count + 101;
                bytes_transferred += block_size;
            }
            // cout << "BUs message: " << message.operation << endl;
            return true;
        }
        return false;
    }

// };

