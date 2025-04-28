#include "core.hpp"
#include "bus.hpp"
#include <vector>
using namespace std;

class Processor {
public:
    // Core* core0;
    // Core* core1;
    // Core* core2;
    // Core* core3;
    // Cache* cache0;
    // Cache* cache1;
    // Cache* cache2;
    // Cache* cache3;
    Core** cores;
    Cache** caches;
    Bus* bus;
    vector<Core*> unblocked_cores;

    Processor(vector<Instruction>** traces, int number_of_sets, int associativity, int block_size) {
        for(int i = 0; i < 4; i++) {
            caches[i] = &Cache(number_of_sets, associativity, block_size);
            cores[i] = &Core(traces[i], caches[i]);
        }
        this->bus = &Bus();
    }

    void simulate() {
        cycle_count++;
        int evict = 0;
        if(cycle_count == bus->when_free){
            bus->is_free;
            CacheLine* line;
            for(int i = 0; i < 4; i++)
            {
                if(bus->change_state[i]) {
                    line = caches[i]->sets[bus->message.index].find_line(bus->message.tag);
                    line->state = bus->change_to[i];
                    bus->change_state[i] = false;
                }
            }
        }
        if(bus->message.operation == RWITM) {
            if(cycle_count == bus->when_free - 100) {
                CacheLine* line;
                for(int i = 0; i < 4; i++) {
                    if(bus->change_state[i]) {
                        if(bus->change_to[i] == INVALID) {
                            line = caches[i]->sets[bus->message.index].find_line(bus->message.tag);
                            line->state = bus->change_to[i];
                            bus->change_state[i] = false;
                        }
                    }
                }
            }
        }

        for(int i = 0; i < 4; i++) {
            if(cycle_count == cores[i]->when_free) {
                cores[i]->is_blocked = false;
            }
        }

        for(int i = 0; i < 4; i++) {
            if (cores[i]->is_blocked) {
                cores[i]->idle_cycles++;
            }
            else {
                cores[i]->run();
                if(cores[i]->result.done) {
                    cores[i]->done = true;
                }
                else{
                    if(cores[i]->result.is_read && cores[i]->result.hit) {
                        //no op
                    }
                    else if(cores[i]->result.is_read && !cores[i]->result.hit) {
                        //eviction logic
                        if(cores[i]->result.state == MODIFIED && !cores[i]->result.hit) {
                            cores[i]->number_of_writebacks++;
                            evict = 100;
                        }

                        BusMessage message = BusMessage{MEMREAD, cores[i]->result.set_index, cores[i]->result.tag, i, true};
                        if(this->bus->request_bus(message)) {
                            cores[i]->is_blocked = true;
                        }
                        else {
                            cores[i]->is_blocked = true;
                        }
                    }
                    else if (!cores[i]->result.is_read && cores[i]->result.hit) {
                        if(cores[i]->result.state == EXCLUSIVE) {
                            cores[i]->result.state = MODIFIED;
                        }
                        else if(cores[i]->result.state == SHARED) {
                            cores[i]->result.state = MODIFIED;
                            BusMessage message = BusMessage{INVALIDATE, cores[i]->result.set_index, cores[i]->result.tag, i, false};
                            if(this->bus->request_bus(message)) {
                                cores[i]->is_blocked = true;
                            }
                            else {
                                cores[i]->is_blocked = true;
                            }
                        }
                    }
                    else if (!cores[i]->result.is_read && !cores[i]->result.hit) {
                        //eviction logic
                        if(cores[i]->result.state == MODIFIED && !cores[i]->result.hit) {
                            cores[i]->number_of_writebacks++;
                            evict = 100;
                        }

                        BusMessage message = BusMessage{RWITM, cores[i]->result.set_index, cores[i]->result.tag, i, false};
                        if(this->bus->request_bus(message)) {
                            cores[i]->is_blocked = true;
                        }
                        else {
                            cores[i]->is_blocked = true;
                        }
                    }
                }
            }
        }

        for(int i = 0; i < 4; i++) {
            if (bus->message.core_id != i) {
                CacheLine* line = caches[i]->sets[bus->message.index].find_line(bus->message.tag);
                State state;
                if (line == nullptr) {
                    state = INVALID;
                } else if (line->state == INVALID) {
                    state = INVALID;
                } else if (line->state == MODIFIED) {
                    state = MODIFIED;
                } else if (line->state == EXCLUSIVE) {
                    state = EXCLUSIVE;
                } else if (line->state == SHARED) {
                    state = SHARED;
                }
                if(bus->message.operation == INVALIDATE) {
                    if (line != nullptr) {
                        bus->change_state[i] = true;
                        bus->change_to[i] = INVALID;
                    }
                }
                else if(bus->message.operation == MEMREAD) {
                    
                    if(state != INVALID) {
                        bus->change_state[i] = true;
                        bus->change_to[i] = SHARED;
                        bus->when_free = cycle_count + 2 * caches[0]->block_size + evict;
                    }
                }
                else if(bus->message.operation == RWITM) {
                    bus->change_state[bus->message.core_id] = true;
                    bus->change_to[bus->message.core_id] = MODIFIED;

                    if(state == EXCLUSIVE || state == SHARED) {
                        bus->change_state[i] = true;
                        bus->change_to[i] = INVALID;
                        bus->when_free = cycle_count + 100 + evict;
                    }
                    else if(state == MODIFIED) {
                        bus->when_free = cycle_count + 200;
                        // how to do SS = I at bus->when_free - 100?
                        bus->change_state[i] = true;
                        bus->change_to[i] = INVALID;
                    }
                }
            }
            
        }










        unblocked_cores.clear();
        for(int i = 0; i < 4; i++) {
            if (!cores[i]->is_blocked) {
                unblocked_cores.push_back(cores[i]);
                cores[i]->run();
            }
        }
        int n = unblocked_cores.size();
        if (n != 0) {
            Core* core_that_wants_bus = nullptr;
            for(int i = 0; i < n; i++) {
                core_that_wants_bus = unblocked_cores[i];
                if (!unblocked_cores[i]->result.hit || (unblocked_cores[i]->result.state == SHARED && !unblocked_cores[i]->result.is_read)) {
                    core_that_wants_bus = unblocked_cores[i];
                    break;
                }
            }
            if (core_that_wants_bus == nullptr) {
                return;
            }
            BusMessage message;
            message.index = core_that_wants_bus->result.set_index;
            message.tag = core_that_wants_bus->result.tag;
            message.is_read = core_that_wants_bus->result.is_read;
            if (core_that_wants_bus == cores[0]) {
                message.core_id = 0;
            } else if (core_that_wants_bus == cores[1]) {
                message.core_id = 1;
            } else if (core_that_wants_bus == cores[2]) {
                message.core_id = 2;
            } else if (core_that_wants_bus == cores[3]) {
                message.core_id = 3;
            }
            if (!core_that_wants_bus->result.is_read && core_that_wants_bus->result.state == SHARED) {
                message.operation = INVALIDATE;
            }
            else {
                message.operation = MEMREAD;
            }
            core_that_wants_bus->is_blocked = true;
            if(bus->request_bus(message)) {
                if (message.operation == INVALIDATE) {
                    for(int i = 0; i < n; i++) {
                        if (cores[i] != core_that_wants_bus) {
                            State state = cores[i]->snoop(message.tag, message.index, false);
                            if (state != INVALID) {
                                cores[i]->update_state(message.tag, message.index, INVALID);
                            }
                        }
                    }
                }
                else {
                    if (message.is_read) {
                        for(int i = 0; i < n; i++) {
                            if (cores[i] != core_that_wants_bus) {
                                State state = cores[i]->snoop(message.tag, message.index, false);
                                if (state == EXCLUSIVE) {
                                    cores[i]->update_state(message.tag, message.index, SHARED);
                                    core_that_wants_bus->update_state(message.tag, message.index, SHARED);
                                    bus->bytes_transferred += caches[i]->block_size;
                                    bus->when_free = cycle_count + 2 * caches[i]->block_size;
                                    bus->is_free = false;
                                }
                                else if (state == SHARED) {
                                    core_that_wants_bus->update_state(message.tag, message.index, SHARED);
                                    bus->bytes_transferred += caches[i]->block_size;
                                    bus->when_free = cycle_count + 2 * caches[i]->block_size;
                                    bus->is_free = false;
                                }
                                else if (state == MODIFIED) {
                                    cores[i]->update_state(message.tag, message.index, SHARED);
                                    core_that_wants_bus->update_state(message.tag, message.index, SHARED);
                                    bus->bytes_transferred += caches[i]->block_size;
                                    bus->when_free = cycle_count + 2 * caches[i]->block_size;
                                    bus->is_free = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            return;
        }
    }
};