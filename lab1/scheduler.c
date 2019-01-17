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

    int* file = (int*)malloc(lines*3*sizeof(int));
    fp = fopen(filename, "r");
    while( ( check = fscanf(fp, "%d %d %d", &pid, &arrival_time, &burst_time) ) != EOF) {
        if( check == 3 ) {
            file[i] = pid;
            file[i+1] = arrival_time;
            file[i+2] = burst_time;
            i += 3;
        }
        else{
            printf("File corrupted!!\n");
            return -1;
        }
    }
    fclose(fp);

    /* Prints the array */
    /*
    i = 0;
    while( i <= 3*sizeof(file) ) {
        printf("%d %d %d", file[i], file[i+1], file[i+2]);
        if( i%3 == 0 ) {
            printf("\n");
        }
        i += 3;
    }*/

    /* Structure for operation of the scheduler. Don't need to touch this. */
    if( argc == 3 && !strcmp(argv[2], "FCFS") ) {
        printf("Now running %s\n", argv[2]);
    }
    else if( argc == 3 && !strcmp(argv[2], "SJF") ) {
        printf("Now running %s\n", argv[2]);
    }
    else if( argc == 4 && !strcmp(argv[2], "RR") && (slice > 0) ) {
        printf("Now running %s with time-slice of %d ms\n", argv[2], slice);
    }
    else{
        printf("Wrong input format:\n");
        printf("./scheduler <filename> <FCFS|SJF|RR> <quantum(only positive integers)>\n");
        return -1;
    }

    return 0;
}