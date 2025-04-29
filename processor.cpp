// #include "core.hpp"
#include "processor.hpp"
#include <vector>
#include <iostream>
using namespace std;

// class Processor {
// public:
    // Core** cores;
    // Cache** caches;
    // vector<Core> cores;
    // vector<Cache> caches;
    // Bus* bus;
    // vector<Core*> unblocked_cores;

    //TODO: choose the data structure for cores and caches

    Processor::Processor(vector<Instruction>(&traces)[4], int number_of_sets, int associativity, int block_size) {
        for(int i = 0; i < 4; i++) {
            Cache* cache = new Cache(number_of_sets, associativity, block_size);
            caches.push_back(cache);
            Core* core = new Core(traces[i], cache);
            cores.push_back(core);
            cout << caches[i]->block_size << endl;
        }
        this->bus = new Bus();
        cout << "line size " << caches[0]->sets[0]->lines.size() << endl;
        cout << "Processor constructor finished" << endl;

    }

    Processor::~Processor() {
        delete bus;
    }

    // Processor::Processor(vector<Instruction>(&traces)[4], int number_of_sets, int associativity, int block_size) {
    //     cout << "Processor constructor called" << endl;
    //     for(int i = 0; i < 4; i++) {
    //         cout << "Creating cache " << i << endl;
    //         caches[i] = new Cache(number_of_sets, associativity, block_size);
    //         cout << "Creating core " << i << endl;
    //         cores[i] = new Core(traces[i], caches[i]);
    //     }
    //     this->bus = new Bus();
    //     cout << "Processor constructor finished" << endl;
    // }

    // Processor::~Processor() {
    //     for (int i = 0; i < 4; i++) {
    //         delete caches[i];
    //         delete cores[i];
    //     }
    //     delete bus;
    // }

    void Processor::simulate() {
        cycle_count++;
        cout << "Simulating cycle " << cycle_count << endl;

        int evict = 0;

        if (!bus->is_free) {
            cout << "Bus is busy" << endl;
            if(cycle_count == bus->when_free) {
                // bus is free now
                // sets the states of the local and snooping caches accordingly 
                bus->is_free = true;
                CacheLine* line_on_bus;
                for(int i = 0; i < 4; i++) {
                    if(bus->change_state[i]) {
                        line_on_bus = caches[i]->sets[bus->message.index]->find_line(bus->message.tag);
                        if(line_on_bus != nullptr) {
                            line_on_bus->state = bus->change_to[i];
                            bus->change_state[i] = false;
                        }
                    }
                }
            }
            if(cycle_count == bus->message.set_local && bus->message.operation == CTOC_THEN_WRITE) {
                CacheLine* line_on_bus = caches[bus->message.local_core_id]->sets[bus->message.index]->find_line(bus->message.tag);
                if(line_on_bus != nullptr) {
                    line_on_bus->state = SHARED;
                    bus->change_state[bus->message.local_core_id] = false;
                }
            }
            if(cycle_count == bus->message.set_remote && bus->message.operation == WRITE_THEN_READ) {
                CacheLine* line_on_bus = caches[bus->message.remote_core_id]->sets[bus->message.index]->find_line(bus->message.tag);
                if(line_on_bus != nullptr) {
                    line_on_bus->state = INVALID;
                    bus->change_state[bus->message.remote_core_id] = false;
                }
            }
            if(bus->message.operation == EVICT_THEN_MEMREAD && cycle_count == bus->when_free - 100) {
                bus->message.operation = MEMREAD;
            }
            if(bus->message.operation == EVICT_THEN_RWITM && cycle_count == bus->when_free - 100) {
                bus->message.operation = RWITM;
            }
        }
        // if(bus->message.operation == RWITM) {
        //     if(cycle_count == bus->when_free - 100) {
        //         CacheLine* line;
        //         for(int i = 0; i < 4; i++) {
        //             if(bus->change_state[i]) {
        //                 if(bus->change_to[i] == INVALID) {
        //                     line = caches[i]->sets[bus->message.index].find_line(bus->message.tag);
        //                     line->state = bus->change_to[i];
        //                     bus->change_state[i] = false;
        //                 }
        //             }
        //         }
        //     }
        // }

        // unblock the core that is using the bus if the operation is done
        // for(int i = 0; i < 4; i++) {
        //     if(cycle_count == cores[i]->when_free) {
        //         cores[i]->is_blocked = false;
        //     }
        // }

        for(int i = 0; i < 4; i++) {
            cout << "Core " << i << " is blocked: " << cores[i]->is_blocked << endl;

            if (cores[i]->is_blocked) {
                // cores[i]->idle_cycles++;
                if (cores[i]->when_free == -1) {
                    // it is waiting for the bus
                    // TODO: request for bus here, if didn't get bus, then increment idle_cycles
                    cores[i]->idle_cycles++;
                } else {
                    // it is using the bus
                    // TODO: check what happens during CTOC_THEN_WRITE and WRITE_THEN_READ
                    if (cycle_count == cores[i]->when_free) {
                        if(cycle_count == 101){cout << "Core " << i << " is unblocked" << endl;}
                        cores[i]->is_blocked = false;
                        cores[i]->when_free = -1;
                        if (bus->message.operation == MEMREAD) {
                            CacheLine* line = caches[i]->sets[bus->message.index]->find_line(bus->message.tag);
                            int addr = cores[i]->trace[cores[i]->current_instr].address;
                            int tag = addr / (block_size * no_of_sets);
                            line->tag = tag;
                        }
                        // cores[i]->current_instr++;
                        
                    }
                }
            }

            if (!cores[i]->is_blocked && !cores[i]->done) {
                cout << "Core " << i << " is not blocked" << endl;
                cores[i]->run();
                cout << "Core " << i << " current instr: " << cores[i]->current_instr << endl;

                cout << "Core " << i << " ran" << endl;
                if(cores[i]->result.done) {
                    // dones means last instruction got finished in last cycle
                    cores[i]->done = true;
                }
                else{
                    // if miss, then check if writeback is needed
                    if(!cores[i]->result.hit) {
                        cout << "Core " << i << " has a miss" << endl;
                        if (cores[i]->result.state == MODIFIED) {
                            cores[i]->number_of_writebacks++;
                            evict = 100;
                        }
                    }

                    if(cores[i]->result.is_read && cores[i]->result.hit) {
                        cout << "Core " << i << " has a read hit" << endl;
                        //READHIT
                        //no op
                        //takes new instruction in next cycle
                    }
                    else if(cores[i]->result.is_read && !cores[i]->result.hit) {
                        //READMISS
                        cout << "Core " << i << " has a read miss" << endl;
                        BusMessage message;
                        if(evict == 0) {
                            // message = BusMessage{MEMREAD, cores[i]->result.set_index, cores[i]->result.tag, i, -1, true, -1, -1};
                            message.operation = MEMREAD;
                            message.index = cores[i]->result.set_index;
                            message.tag = cores[i]->result.tag;
                            message.local_core_id = i;
                            message.remote_core_id = -1;
                            message.is_read = true;
                            message.set_local = -1;
                            message.set_remote = -1;
                            bus->change_state[i] = true;
                            bus->change_to[i] = EXCLUSIVE;
                        } else {
                            // message = BusMessage{EVICT_THEN_MEMREAD, cores[i]->result.set_index, cores[i]->result.tag, i, -1, true, -1, -1};
                            message.operation = EVICT_THEN_MEMREAD;
                            message.index = cores[i]->result.set_index;
                            message.tag = cores[i]->result.tag;
                            message.local_core_id = i;
                            message.remote_core_id = -1;
                            message.is_read = true;
                            message.set_local = -1;
                            message.set_remote = -1;
                        }
                        if(this->bus->request_bus(message)) {
                            cores[i]->is_blocked = true;
                            cores[i]->when_free = bus->when_free;
                        }
                        else {
                            cores[i]->is_blocked = true;
                            cores[i]->when_free = -1;
                        }
                    }
                    else if (!cores[i]->result.is_read && cores[i]->result.hit) {
                        //WRITEHIT
                        cout << "Core " << i << " has a write hit" << endl;
                        if(cores[i]->result.state == EXCLUSIVE) {
                            cores[i]->result.state = MODIFIED;
                        }
                        else if(cores[i]->result.state == SHARED) {
                            cores[i]->result.state = MODIFIED;
                            BusMessage message;
                            // BusMessage message = BusMessage{INVALIDATE, cores[i]->result.set_index, cores[i]->result.tag, i, -1, false, -1, -1};
                            message.operation = INVALIDATE;
                            message.index = cores[i]->result.set_index;
                            message.tag = cores[i]->result.tag;
                            message.local_core_id = i;
                            message.remote_core_id = -1;
                            message.is_read = false;
                            message.set_local = -1;
                            message.set_remote = -1;
                            
                            if(this->bus->request_bus(message)) {
                                // cores[i]->is_blocked = true;
                                cores[i]->is_blocked = false;
                            }
                            else {
                                cores[i]->is_blocked = true;
                                cores[i]->when_free = -1;
                            }
                        }
                    }
                    else if (!cores[i]->result.is_read && !cores[i]->result.hit) {
                        //WRITEMISS
                        cout << "Core " << i << " has a write miss" << endl;
                        BusMessage message;
                        if(evict == 0) {
                            // message = BusMessage{RWITM, cores[i]->result.set_index, cores[i]->result.tag, i, -1, false, -1, -1};
                            message.operation = RWITM;
                            message.index = cores[i]->result.set_index;
                            message.tag = cores[i]->result.tag;
                            message.local_core_id = i;
                            message.remote_core_id = -1;
                            message.is_read = false;
                            message.set_local = -1;
                            message.set_remote = -1;
                        } else {
                            // message = BusMessage{EVICT_THEN_RWITM, cores[i]->result.set_index, cores[i]->result.tag, i, -1, false, -1, -1};
                            message.operation = EVICT_THEN_RWITM;
                            message.index = cores[i]->result.set_index;
                            message.tag = cores[i]->result.tag;
                            message.local_core_id = i;
                            message.remote_core_id = -1;
                            message.is_read = false;
                            message.set_local = -1;
                            message.set_remote = -1;
                        }

                        if(this->bus->request_bus(message)) {
                            cores[i]->is_blocked = true;
                            cores[i]->when_free = bus->when_free;
                        } else {
                            cores[i]->is_blocked = true;
                            cores[i]->when_free = -1;
                        }
                    }
                }
            }
            cout << "when free for core " << i << ":" << cores[i]->when_free << endl;
        }

        for(int i = 0; i < 4; i++) {
            if (bus->message.local_core_id != i) {
                CacheLine* line = caches[i]->sets[bus->message.index]->find_line(bus->message.tag);

                State state_in_remote_cache;
                if (line == nullptr) {
                    state_in_remote_cache = INVALID;
                } else if (line->state == INVALID) {
                    state_in_remote_cache = INVALID;
                } else if (line->state == MODIFIED) {
                    state_in_remote_cache = MODIFIED;
                } else if (line->state == EXCLUSIVE) {
                    state_in_remote_cache = EXCLUSIVE;
                } else if (line->state == SHARED) {
                    state_in_remote_cache = SHARED;
                }

                if(bus->message.operation == INVALIDATE) {
                    if (line != nullptr) {
                        bus->change_state[i] = true;
                        bus->change_to[i] = INVALID;
                        bus->change_state[bus->message.local_core_id] = true;
                        bus->change_to[bus->message.local_core_id] = MODIFIED;
                    }
                }
                else if(bus->message.operation == MEMREAD) {
                    if(state_in_remote_cache == EXCLUSIVE) {
                        bus->message.operation = C_TO_C;
                        bus->when_free = cycle_count + 2 * caches[0]->block_size;
                        bus->change_state[i] = true;
                        bus->change_state[bus->message.local_core_id] = true;
                        bus->change_to[i] = SHARED;
                        bus->change_to[bus->message.local_core_id] = SHARED;
                    }
                    else if(state_in_remote_cache == SHARED) {
                        bus->message.operation = C_TO_C;
                        bus->when_free = cycle_count + 2 * caches[0]->block_size;
                        bus->change_state[bus->message.local_core_id] = true;
                        bus->change_to[bus->message.local_core_id] = SHARED;
                    }
                    else if(state_in_remote_cache == MODIFIED) {
                        bus->message.operation = CTOC_THEN_WRITE;
                        bus->when_free = cycle_count + 2 * caches[0]->block_size + 100;
                        bus->message.set_local = cycle_count + 2 * caches[0]->block_size;
                        bus->message.remote_core_id = i;
                        bus->change_state[i] = true;
                        bus->change_to[i] = SHARED;
                        bus->change_state[bus->message.local_core_id] = true;
                        bus->change_to[bus->message.local_core_id] = SHARED;
                    } else {}
                }
                else if(bus->message.operation == RWITM) {
                    // bus->change_state[bus->message.local_core_id] = true;
                    // bus->change_to[bus->message.local_core_id] = MODIFIED;

                    // if(state == EXCLUSIVE || state == SHARED) {
                    //     bus->change_state[i] = true;
                    //     bus->change_to[i] = INVALID;
                    //     bus->when_free = cycle_count + 100 + evict;
                    // }
                    // else if(state == MODIFIED) {
                    //     bus->when_free = cycle_count + 200;
                    //     // how to do SS = I at bus->when_free - 100?
                    //     bus->change_state[i] = true;
                    //     bus->change_to[i] = INVALID;
                    // }

                    if(state_in_remote_cache == EXCLUSIVE || state_in_remote_cache == SHARED) {
                        bus->change_state[i] = true;
                        bus->change_to[i] = INVALID;
                        bus->change_state[bus->message.local_core_id] = true;
                        bus->change_to[bus->message.local_core_id] = MODIFIED;
                    }
                    else if(state_in_remote_cache == MODIFIED) {
                        bus->message.operation = WRITE_THEN_READ;
                        bus->when_free = cycle_count + 200;
                        bus->message.set_remote = cycle_count + 100;
                        bus->message.remote_core_id = i;
                        bus->change_state[i] = true;
                        bus->change_to[i] = INVALID;
                        bus->change_state[bus->message.local_core_id] = true;
                        bus->change_to[bus->message.local_core_id] = MODIFIED;
                    }
                }
            }
            
        }

        // unblocked_cores.clear();
        // for(int i = 0; i < 4; i++) {
        //     if (!cores[i]->is_blocked) {
        //         unblocked_cores.push_back(cores[i]);
        //         cores[i]->run();
        //     }
        // }
        // int n = unblocked_cores.size();
        // if (n != 0) {
        //     Core* core_that_wants_bus = nullptr;
        //     for(int i = 0; i < n; i++) {
        //         core_that_wants_bus = unblocked_cores[i];
        //         if (!unblocked_cores[i]->result.hit || (unblocked_cores[i]->result.state == SHARED && !unblocked_cores[i]->result.is_read)) {
        //             core_that_wants_bus = unblocked_cores[i];
        //             break;
        //         }
        //     }
        //     if (core_that_wants_bus == nullptr) {
        //         return;
        //     }
        //     BusMessage message;
        //     message.index = core_that_wants_bus->result.set_index;
        //     message.tag = core_that_wants_bus->result.tag;
        //     message.is_read = core_that_wants_bus->result.is_read;
        //     if (core_that_wants_bus == cores[0]) {
        //         message.core_id = 0;
        //     } else if (core_that_wants_bus == cores[1]) {
        //         message.core_id = 1;
        //     } else if (core_that_wants_bus == cores[2]) {
        //         message.core_id = 2;
        //     } else if (core_that_wants_bus == cores[3]) {
        //         message.core_id = 3;
        //     }
        //     if (!core_that_wants_bus->result.is_read && core_that_wants_bus->result.state == SHARED) {
        //         message.operation = INVALIDATE;
        //     }
        //     else {
        //         message.operation = MEMREAD;
        //     }
        //     core_that_wants_bus->is_blocked = true;
        //     if(bus->request_bus(message)) {
        //         if (message.operation == INVALIDATE) {
        //             for(int i = 0; i < n; i++) {
        //                 if (cores[i] != core_that_wants_bus) {
        //                     State state = cores[i]->snoop(message.tag, message.index, false);
        //                     if (state != INVALID) {
        //                         cores[i]->update_state(message.tag, message.index, INVALID);
        //                     }
        //                 }
        //             }
        //         }
        //         else {
        //             if (message.is_read) {
        //                 for(int i = 0; i < n; i++) {
        //                     if (cores[i] != core_that_wants_bus) {
        //                         State state = cores[i]->snoop(message.tag, message.index, false);
        //                         if (state == EXCLUSIVE) {
        //                             cores[i]->update_state(message.tag, message.index, SHARED);
        //                             core_that_wants_bus->update_state(message.tag, message.index, SHARED);
        //                             bus->bytes_transferred += caches[i]->block_size;
        //                             bus->when_free = cycle_count + 2 * caches[i]->block_size;
        //                             bus->is_free = false;
        //                         }
        //                         else if (state == SHARED) {
        //                             core_that_wants_bus->update_state(message.tag, message.index, SHARED);
        //                             bus->bytes_transferred += caches[i]->block_size;
        //                             bus->when_free = cycle_count + 2 * caches[i]->block_size;
        //                             bus->is_free = false;
        //                         }
        //                         else if (state == MODIFIED) {
        //                             cores[i]->update_state(message.tag, message.index, SHARED);
        //                             core_that_wants_bus->update_state(message.tag, message.index, SHARED);
        //                             bus->bytes_transferred += caches[i]->block_size;
        //                             bus->when_free = cycle_count + 2 * caches[i]->block_size;
        //                             bus->is_free = false;
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }
        // else {
        //     return;
        // }
    }
