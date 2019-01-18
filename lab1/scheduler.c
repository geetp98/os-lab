#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

int isdigit2(char *s) {
    /* Convert a character array into integer,
       but only after verification that
       input contains integers only.
       Otherwise return -1. */
    unsigned int i = 0;
    unsigned int n = strlen(s);
    int sum = 0;
    for(;i < n; i++){
        if( s[i] > '0' && s[i] < '9') {
            sum = sum * 10 + (s[i] - '0');
        }
        else{
            return -1;
        }
    }
    return sum;
}

int main(int argc, char **argv){
    
    ///////////////////////////////////////////////////
    /* input methods until line 77. Don't touch that.*/
    int check = 0;
    int i = 0;
    int lines = 0;
    char c;

    int slice;
    if( argc == 4 ){
        slice = isdigit2(argv[3]);
    }

    /* Convert a file into an array of integers */

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

    int file[3][lines];
    
    fp = fopen(filename, "r");
    while( ( check = fscanf(fp, "%d %d %d", &pid, &arrival_time, &burst_time) ) != EOF) {
        if( check == 3 ) {
            //printf("%d %d %d\n", pid, arrival_time, burst_time);
            file[0][i] = pid;
            file[1][i] = arrival_time;
            file[2][i] = burst_time;
            //printf("%d %d %d\n", file[0][i], file[1][i], file[2][i]);
            i += 1;
        }
        else{
            printf("File corrupted!!\n");
            return -1;
        }
    }
    fclose(fp);

    /* Prints the array
    i = 0;
    while( i < lines ) {
        printf("%d %d %d\n", file[0][i], file[1][i], file[2][i]);
        i++;
    }*/
    ///////////////////////////////////////////////////
    /* This is where the fun begins.
       A random Star Wars quote.
       Yes, I like prequels and dont judge me for that.*/

    if( argc == 3 && !strcmp(argv[2], "FCFS") ) {
        printf("Scheduling Algorithm: %s\n", argv[2]);
        /*int time = 0;
        int total_time = 0;
        i = 0;
        while(i < lines){
            total_time = total_time + file[2][i] - file[1][i];
            i++;
        }
        if(total_time > file[])*/
    }
    else if( argc == 3 && !strcmp(argv[2], "SJF") ) {
        printf("Scheduling Algorithm: %s\n", argv[2]);
    }
    else if( argc == 4 && !strcmp(argv[2], "RR") && (slice > 0) ) {
        printf("Scheduling Algorithm: %s with time-slice of %d ms\n", argv[2], slice);
    }
    else{
        printf("Wrong input format:\n");
        printf("./scheduler <filename> <FCFS|SJF|RR> <quantum(only positive integers)>\n");
        return -1;
    }

    return 0;
}
