#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#include "utils.hpp"
#include "processor.hpp"
using namespace std;

vector<Instruction> parse_trace (ifstream& f);

int main(int argc, char* argv[]) {
    int skip = 1;
    int count = 0;
    // while loop in whichfirst we will check the blocked cores, then whcihc cores want to access the bus, and then we will sequentialize the remaining cores and randomize the cores that want to access the bus. will run till all the cores instructions have finished

    // output according to flag

    string name_of_app;
    string name_of_output_file;
    
    ifstream f;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 't':
                    name_of_app = argv[i + 1];
                    break;
                case 's':
                    no_of_sets = (int)pow(2, atoi(argv[i + 1]));
                    break;
                case 'E':
                    no_of_blocks = atoi(argv[i + 1]);
                    break;
                case 'b':
                    block_size = (int)pow(2, atoi(argv[i + 1]));
                    break;
                case 'o':
                    name_of_output_file = argv[i + 1];
                    break;
                case 'h':
                    cout << "-t <tracefile>: name of parallel application (e.g. app1) whose 4 traces are to be used in simulation" << endl;
                    cout << "-s <s>: number of set index bits (number of sets in the cache = S = 2^s)" << endl;
                    cout << "-E <E>: associativity (number of cache lines per set)" << endl;
                    cout << "-b <b>: number of block bits (block size = B = 2^b)" << endl;
                    cout << "-o: <outfilename> logs output in file for plotting etc." << endl;
                    cout << "-h: prints this help" << endl;
                    break;
            }
        }
    }

    vector<Instruction> instructions[4];

    vector<string> trace_files;
    trace_files.push_back(name_of_app + "_proc0.trace");
    trace_files.push_back(name_of_app + "_proc1.trace");
    trace_files.push_back(name_of_app + "_proc2.trace");
    trace_files.push_back(name_of_app + "_proc3.trace");

    for (int i = 0; i < 4; i++) {
        ifstream f(trace_files[i]);
        if (!f.is_open()) {
            cerr << "Error: Could not open file " << trace_files[i] << endl;
            return 1;
        }
        instructions[i] = parse_trace(f);
        f.close();
    }


    Processor processor = Processor(instructions, no_of_sets, no_of_blocks, block_size);

    while(true) {
        if (processor.cores[0]->done && processor.cores[1]->done && processor.cores[2]->done && processor.cores[3]->done) {
            break;
        } else {
            processor.simulate();
            count ++;
            
            // if(count >= skip) {
            //     count = 0;
            //     cin >> skip;
            // }
            // int foo; cin >> foo;
        }
    }

    // for(int i = 0; i < 102; i++) {
    //     processor.simulate();
    // }
    // processor.simulate();

    for (int i = 0; i < 4; i++){
        cout << "Core " << i << " statistics:" << endl;
        cout << "Total instructions: " << instructions[i].size() << endl;
        cout << "Total reads: " << processor.cores[i]->number_of_reads << endl;
        cout << "Total writes: " << processor.cores[i]->number_of_writes << endl;
        cout << "Total cycles: " << processor.cores[i]->total_execution_cycles << endl;
        cout << "Idle cycles: " << processor.cores[i]->idle_cycles << endl;
        cout << "Cache misses: " << processor.caches[i]->number_of_misses << endl;
        cout << "Miss rate: " << (double)processor.caches[i]->number_of_misses / instructions[i].size() * 100 << "%" << endl;
        cout << "Cache evictions: " << processor.caches[i]->number_of_evictions << endl;
        //TODO: choose one of the following
        cout << "Writebacks cache: " << processor.caches[i]->number_of_writebacks << endl;
        cout << "Writebacks core: " << processor.cores[i]->number_of_writebacks << endl;
        //TODO: bus statistics are per core, not overall. fix it.
        cout << "Bus invalidations: " << processor.cores[i]->bus_invalidations << endl;
        cout << "Data traffic: " << processor.cores[i]->traffic << endl;
        cout << endl;
    }
    


    return 0;
}

//TODO: count number of reads and writes

vector<Instruction> parse_trace (ifstream& f) {
    vector<Instruction> instructions;
    string line;

    while(getline(f, line)) {
        if (line.empty()) continue;

        if (line[0] != 'R' && line[0] != 'W') {
            cerr << "Warning: skipping invalid line: " << line << endl;
            continue;
        }

        size_t space_pos = line.find(' ');
        if (space_pos == string::npos || space_pos + 1 >= line.size()) {
            cerr << "Warning: skipping invalid line: " << line << endl;
            continue;
        }

        char type = line[0];
        string addr_str = line.substr(space_pos + 1);

        try {
            Instruction instr;
            instr.is_read = (type == 'R');
            instr.address = static_cast<int>(stoul(addr_str, nullptr, 16));
            instructions.push_back(instr);
        } catch (const invalid_argument& e) {
            cerr << "Warning: invalid address in line: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Warning: address out of range in line: " << line << endl;
            continue;
        }
    }

    return instructions;

    // while(!f.eof()) {
    //     getline(f,line);
    //     bool read = true;
    //     if (line[0] == 'W') {
    //         read = false;
    //     }
    //     int i = 1;
    //     while(line[i] == ' ' || line[i] == '\t') {
    //         i++;
    //     }
    //     int j = line.length() - 1;
    //     while(line[j] == ' ' || line[j] == '\t') {
    //         j--;
    //     }
    //     string hex_addr = line.substr(i, j - i + 1);
    //     int mem_addr = hex_to_int(hex_addr);
    //     Instruction in;
    //     in.memory_addr = mem_addr;
    //     in.read = read;
    //     vec.push_back(in);
    // }
    // f.close();
    // return vec;
}

