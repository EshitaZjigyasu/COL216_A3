#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "utils.hpp"
#include "processor.hpp"
using namespace std;

// vector<Instr> parse_trace (ifstream f);


int main(int argc, char* argv[]) {
    //first check if h flag
    // take traces as inputs

    // parse the trace files and make a list of tuples (int and char)

    // while loop in whichfirst we will chekc the blocked cores, then whcihc cores want to access the bus, and then we will sequentialize the remaining cores and randomize the cores that want to access the bus. will run till all the cores instructions have finished

    // output according to flag
    int no_of_sets = 64; 
    int no_of_blocks = 2;
    int block_size = 32;
    ifstream f;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 't':
                    //open the four trace files
                    break;
                case 's':
                    no_of_sets = atoi(argv[i + 1]);
                    break;
                case 'E':
                    no_of_blocks = atoi(argv[i + 1]);
                    break;
                case 'b':
                    block_size = atoi(argv[i + 1]);
                    break;
                case 'o':
                    // open file for output
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

    vector<Instruction> instructions0 = parse_trace(ifstream("trace0.txt"));
    vector<Instruction> instructions1 = parse_trace(ifstream("trace1.txt"));
    vector<Instruction> instructions2 = parse_trace(ifstream("trace2.txt"));
    vector<Instruction> instructions3 = parse_trace(ifstream("trace3.txt"));

    Processor processor = Processor(&instructions0, &instructions1, &instructions2, &instructions3, no_of_sets, no_of_blocks, block_size);

    while(true) {
        if (processor.core0->done && processor.core1->done && processor.core2->done && processor.core3->done) {
            break;
        } else {
            processor.simulate();
        }
    }

    return 0;
}

//TODO: count number of reads and writes

vector<Instruction> parse_trace (ifstream f) {
    vector<Instruction> vec;
    string line;
    while(!f.eof()) {
        getline(f,line);
        bool read = true;
        if (line[0] == 'W') {
            read = false;
        }
        int i = 1;
        while(line[i] == ' ' || line[i] == '\t') {
            i++;
        }
        int j = line.length() - 1;
        while(line[j] == ' ' || line[j] == '\t') {
            j--;
        }
        string hex_addr = line.substr(i, j - i + 1);
        int mem_addr = hex_to_int(hex_addr);
        Instruction in;
        in.memory_addr = mem_addr;
        in.read = read;
        vec.push_back(in);
    }
    f.close();
    return vec;
}

