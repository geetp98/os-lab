#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

const int opt_i = 1;
const int fifo_i = 2;
const int lru_i = 3;

int misses_opt = 0;
int misses_fifo = 0;
int misses_lru = 0;
int total_access = 0;

using namespace std;

int isdigit2(const char *s) {
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

int chosen_algo(const char *s){       // returns appropriate integer with
    if(!strcmp(s, "opt")){      // respect to input of algorithms
        return opt_i;
    }
    if(!strcmp(s, "fifo")){
        return fifo_i;
    }
    if(!strcmp(s, "lru")){
        return lru_i;
    }
    return -1;
}

int max_vector(vector<int> v){
    int n = v[0];
    int i = 1;
    while(i < v.size()){
        if(v[i] > v[i-1]){
            n = v[i];
        }
        i++;
    }
    return n;
}

int main2(int physical_pfs, const char* input__, char* algo__){
    unsigned int i = 0;
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    int algo = chosen_algo(algo__);            // Number of the chosen one. 1-opt, 2-fifo, 3-lru

    ifstream myfile;
    string line;
    myfile.open(input__);
    getline (myfile, line);

    vector<int> access_list(0,0);
    std::vector<int>::iterator it = access_list.begin();
    char *char_a;
    i = 0;
    int prev = 0;
    total_access = 0;
    while(i < line.size()){
        if(line[i] == ' '){
            total_access += 1;
            if( isdigit2((char*)line.substr(prev, i - prev).c_str()) == -1 ){
                cout << "File corrupted. Try again. Now Exiting..";
                exit(0);
            }
            access_list.insert(access_list.end(), isdigit2((char*)line.substr(prev, i - prev).c_str()));
            prev = i+1;
        }
        i += 1;
    }

    i = line.size();
    while(line[i] != ' '){
        prev = i;
        i -= 1;
    }
    access_list.insert(access_list.end(), isdigit2((char*)line.substr(prev, i - prev).c_str()));

    if(algo == 1){      // opt_i:done
        list<int> page_list;
        misses_opt = 0;
        ;
        int fault = 0;
        while(access_list.size()){
            fault = 0;
            if( !(find(page_list.begin(), page_list.end(), access_list.front()) != page_list.end()) ){
                fault = 1;
                misses_opt += 1;
                if(page_list.size() >= physical_pfs){
                    vector<int> index;
                    vector<int>:: iterator it2 = access_list.begin();
                    while(it2 != access_list.end()){
                        list<int>::iterator it = find(page_list.begin(), page_list.end(), *it2);
                        if(*it == *it2){
                            index.insert(index.begin(), *it);
                        }
                        it2++;
                    }
                    if(!index.empty()){
                        page_list.remove(index.front());
                    }
                    else{
                        page_list.pop_front();
                    }
                }
                page_list.push_back(access_list.front());
            }
            access_list.erase(access_list.begin());
        }
    }
    else if(algo == 2){      // fifo_i:done
        list<int> page_list;
        misses_fifo = 0;
        ;
        int fault = 0;
        while(access_list.size()){
            fault = 0;
            if( !(find(page_list.begin(), page_list.end(), access_list.front()) != page_list.end()) ){
                fault = 1;
                misses_fifo += 1;
                if(page_list.size() >= physical_pfs){
                    page_list.pop_front();
                }
                page_list.push_back(access_list.front());
            }
            access_list.erase(access_list.begin());
        }
    }
    else if(algo == 3){      // lru_i:done
        list<int> page_list;
        misses_lru = 0;
        ;
        int fault = 0;
        int p;
        while(access_list.size()){
            fault = 0;
            if( !(find(page_list.begin(), page_list.end(), access_list.front()) != page_list.end()) ){
                fault = 1;
                misses_lru += 1;
                if(page_list.size() >= physical_pfs){
                    page_list.pop_front();
                }
                page_list.push_back(access_list.front());
            }
            else{
                p = access_list.front();
                page_list.remove(p);
                page_list.push_back(p);
            }
            access_list.erase(access_list.begin());
        }
    }

    return 0;
}