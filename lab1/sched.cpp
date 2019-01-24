#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct process{
    int pid;
    int arr_time;
    int burst_time;
};

bool Comapare_fcfs(process p1, process p2)
{
    return p1.arr_time > p2.arr_time;
}
bool Compare_sjf(process p1, process p2)
{
    /* work */
    return true;
}
bool Compare_rr(process p1, process p2)
{
    /* work */
    return true;
}

int isdigit2(char *s) {
    /* Convert a character array into integer,
       but only after verification that
       input contains integers only.
       Otherwise return -1. */
    unsigned int i = 0;
    unsigned int n = strlen(s);
    int sum = 0;
    for (;i < n; i++) {
        if( s[i] > '0' && s[i] < '9') {
            sum = sum * 10 + (s[i] - '0');
        }
        else {
            return -1;
        }
    }
    return sum;
}

int main(int argc, char **argv){
    int check = 0;
    int i = 0;
    int lines = 1;
    char c;

    int slice;
    if( argc == 4 ){
        slice = isdigit2(argv[3]);
    }

    /* Convert a suitable file into an array of integers */
    int pid, arrival_time, burst_time;
    char *filename = argv[1];
    FILE *fp;
    fp = fopen(filename, "r");
    for (c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n'){
            lines = lines + 1;
        }
    }
    fclose(fp);

    fp = fopen(filename, "r");
    int file[3][lines];
    while( ( check = fscanf(fp, "%d %d %d", &pid, &arrival_time, &burst_time) ) != EOF) {
        if ( check == 3 ) {
            file[0][i] = pid;
            file[1][i] = arrival_time;
            file[2][i] = burst_time;
            i += 1;
        }
        else {
            printf("File corrupted!!\n");
            return -1;
        }
    }
    fclose(fp);

    cout << "Scheduling algorithm : " << argv[2] << endl;
    cout << "Total " << lines << " tasks read from " << argv[1] << ". Now starting..." << endl;
    cout << "==============================================" << endl;

    if ( argc == 3 && !strcmp(argv[2], "FCFS") ) {
        queue<process> myq;
        int systime  = 0;
        int last_add = 0;
        while(true){
            while ( file[1][last_add] == systime ) {
                process p1;
                p1.pid = file[0][last_add]; p1.arr_time = file[1][last_add]; p1.burst_time = file[2][last_add];

                last_add += 1;
            }
        }
    }
    else if ( argc == 3 && !strcmp(argv[2], "SJF") ) {
        /* work */
    }
    else if ( argc == 4 && !strcmp(argv[2], "RR") && (slice > 0) ) {
        /* work */
    }
    else {
        cout << "Wrong input format: Use one as shown below." << endl;
        cout << "./scheduler <filename> <FCFS|SJF|RR> <quantum (only positive integers) (Only for RR)>" << endl;
        return -1;
    }

    return 0;
}