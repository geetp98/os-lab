#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <list>

const int opt = 1;
const int fifo = 2;
const int lru = 3;

using namespace std;

int isdigit2(char *s) {
    /* Convert a character array into integer,
       but only after verification that
       input contains integers only.
       Otherwise return -1. */
    unsigned int i = 0;
    unsigned int n = strlen(s);
    int sum = 0;
    for (;i < n; i++) {
        if( s[i] >='0' && s[i] <= '9') {
            sum = sum * 10 + (s[i] - '0');
        }
        else {
            return -1;
        }
    }
    return sum;
}

int chosen_algo(char *s){       // returns appropriate integer with
    if(!strcmp(s, "opt")){      // respect to input of algorithms
        return opt;
    }
    if(!strcmp(s, "fifo")){
        return fifo;
    }
    if(!strcmp(s, "lru")){
        return lru;
    }
    if(!strcmp(s, "anakin")){
        cout << "I find your lack of faith disturbing." << endl;
    }
    return -1;
}

int main(int argc, char** argv){
    unsigned int i = 0;

    int physical_pfs = isdigit2(argv[1]);       // Number of physical page frames
    char* input__ = argv[2];                    // Name of the input file
    int algo = chosen_algo(argv[3]);            // Number of the chosen one. 1-opt, 2-fifo, 3-lru, enter anakin for a surprise
    int verbose__ = isdigit2(argv[4]);          // verbose

    if(physical_pfs > 100 || physical_pfs < 1){
        cout << "Physical page frames must be between 2 and 100" << endl;
        return 0;
    }
    if(algo == -1){
        if(!strcmp(argv[3], "anakin")){
            return 0;
        }
        cout << "No algorithm named " << argv[3] << "\nPlease enter one of 'opt', 'fifo' or 'lru'" << endl;
        return 0;
    }
    if(verbose__ != 0 && verbose__ != 1){
        cout << "Enter proper value of verbose, either 1 or 0" << endl;
        return 0;
    }

    ifstream myfile;
    string line;
    myfile.open(input__);
    getline (myfile, line);

    vector<int> access_list(0,0);
    std::vector<int>::iterator it = access_list.begin();
    int total_access = 0;
    char *char_a;
    i = 0;
    int prev = 0;
    while(i < line.size()){
        if(line[i] == ' '){
            total_access += 1;
            access_list.insert(access_list.end(), isdigit2((char*)line.substr(prev, i - prev).c_str()));
            prev = i+1;
        }
        i += 1;
    }
    total_access += 1;

    i = line.size();
    while(line[i] != ' '){
        prev = i;
        i -= 1;
    }
    access_list.insert(access_list.end(), isdigit2((char*)line.substr(prev, i - prev).c_str()));

    cout << "----------------------------------------" << endl;
    cout << "Starting now..." << endl;
    cout << "  Algorithm: " << argv[3] << endl;
    cout << "  Physical page frames: " << physical_pfs << endl;
    cout << "  Reading inputs from the file: " << input__ << endl;
    cout << "  Total inputs read: " << total_access << endl;
    cout << "  Input order: ";
    std::copy(access_list.begin(), access_list.end(), std::ostream_iterator<int>(std::cout, " "));
    cout << endl;
    if(verbose__){
        cout << "  Verbose: On" << endl;
    }
    else{
        cout << "  Verbose: Off" << endl;
    }

    if(algo == 1){      // opt
        return 0;
    }
    if(algo == 2){      // fifo
        return 0;
    }
    if(algo == 3){      // lru
        return 0;
    }

    cout << "----------------------------------------" << endl;
    return 0;
}