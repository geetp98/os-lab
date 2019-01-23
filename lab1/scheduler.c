#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>


/* Here is a bad implementation of queue
   where important pointers like front and
   rear are just hanging around */
int pq[INT16_MAX][4];
int front;
int rear;

void create(){
    front = -1;
    rear = -1;
    return;
}

void insert(int n1, int n2, int n3, int p){
    front = 0;
    if (front == -1 && rear == -1) {
        pq[rear][0] = n1;
        pq[rear][1] = n2;
        pq[rear][2] = n3;
        pq[rear][3] = p;
        rear += 1;
        return;
    }
    else{
        int i;
        i = rear;
        int j = 0;
        while (p > pq[i][3] && i >= 0) {
            pq[i+1][0] = pq[i][0];
            pq[i+1][1] = pq[i][1];
            pq[i+1][2] = pq[i][2];
            pq[i+1][3] = pq[i][3];
            i -= 1;
        }
        i += 1;
        pq[i][0] = n1;
        pq[i][1] = n2;
        pq[i][2] = n3;
        pq[i][3] = p;
        rear += 1;
        return;
    }
}

void rem(){
    int i = front;

    while (i != rear) {
        //printf("i %d\n", i);
        pq[i][0] = pq[i+1][0];
        pq[i][1] = pq[i+1][1];
        pq[i][2] = pq[i+1][2];
        pq[i][3] = pq[i+1][3];
        i+=1;
    }
    pq[rear][0] = 0;
    pq[rear][1] = 0;
    pq[rear][2] = 0;
    pq[rear][3] = 0;
    rear -= 1;

    if(rear == -1){
        front = -1;
    }

    return;
}

void reprioritize() {
    int i = front;
    while ( i != rear ) {
        if ( pq[i][3] < pq[i+1][3]) {
            int temp = pq[i][0];
            pq[i][0] = pq[i+1][0];
            pq[i+1][0] = temp;

            temp = pq[i][1];
            pq[i][1] = pq[i+1][1];
            pq[i+1][1] = temp;

            temp = pq[i][2];
            pq[i][2] = pq[i+1][2];
            pq[i+1][2] = temp;

            temp = pq[i][3];
            pq[i][3] = pq[i+1][3];
            pq[i+1][3] = temp;
        }
        i += 1;
    }
    return;
}

bool isqempty(){
    if(front == -1 && rear == -1){
        return true;
    }
    return false;
}

void disp(){
    int i = 0;
    if (front == -1 && rear == -1) {
        printf("Queue is empty!!\n");
        return;
    }
    else{
        front = 0;
        while (i <= rear) {
            printf("%d %d %d %d -> ", pq[i][0], pq[i][1], pq[i][2], pq[i][3]);
            i += 1;
        }
        printf("\n");
        return;
    }
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
    
    ///////////////////////////////////////////////////
    /* input filtering until line **insert something. Don't touch before that.*/
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

    printf("Scheduling algorithm: %s\n", argv[2]);
    printf("Total %d tasks read from %s. Now starting...\n", lines, argv[1]);
    printf("==============================================\n");
    
    /*i = 0;
    while( i < lines ) {
        printf("%d %d %d\n", file[0][i], file[1][i], file[2][i]);
        i++;
    }*/
    ///////////////////////////////////////////////////
    /* This is where the fun begins.
       A random Star Wars quote.
       Yes, I like prequels and dont judge me for that.*/

    if ( argc == 3 && !strcmp(argv[2], "FCFS") ) {
        i = 0;
        create();
        while (i < lines) {
            insert(file[0][i], file[1][i], file[2][i], -1*file[1][i]);
            i += 1;
        }
        int systime = 0;
        while ( !isqempty() ) {
            int time_left = pq[front][2];
            int next_arrival_time = pq[front+1][1];
            while( time_left > 0 ){
                printf("<system time %d> process %d running..\n", systime, pq[front][0]);
                systime += 1;
                time_left -= 1;
            }
            printf("<system time %d> process %d finished......\n", systime, pq[front][0]);
            while ( next_arrival_time > systime ) {
                systime += 1;
                printf("<system time %d> waiting for a process\n", systime);
            }
            rem();
        }
    }
    else if ( argc == 3 && !strcmp(argv[2], "SJF") ) {
        i = 0;
        create();
        int systime = 0;
        int last_add = 0;
        while (true) {
            while ( file[1][last_add] == systime ) {
                insert(file[0][last_add], file[1][last_add], file[2][last_add], -1*file[2][last_add]);
                last_add += 1;
            }
            pq[front][2] -= 1;
            pq[front][3] += 1;
            systime += 1;
            if ( pq[front][2] == 0 ) {
                printf("<system time %d> process %d finished......\n", systime, pq[front][0]);
                rem();
            }
            else if ( !isqempty() ) {
                printf("<system time %d> process %d running..\n", systime, pq[front][0]);
            }
            reprioritize();
            if ( isqempty() && last_add >= lines) {
                break;
            }
            else if ( isqempty() && last_add < lines ) {
                printf("<system time %d> waiting for a process\n", systime);
            }
        }
    }
    else if ( argc == 4 && !strcmp(argv[2], "RR") && (slice > 0) ) {
    }
    else {
        printf("Wrong input format:\n");
        printf("./scheduler <filename> <FCFS|SJF|RR> <quantum(only positive integers)>\n");
        return -1;
    }

    /////////////////////////
    /* this place is reserved for queue. do not mess 
    create();
    printf("%d\n",isqempty());
    disp();
    insert(1, 2, 3, 4);
    disp();
    printf("%d\n",isqempty());
    insert(1, 3, 5, 5);
    disp();
    insert(1, 4, 6, 7);
    disp();
    rem();
    disp();
    rem();
    disp();
    insert(1, 4, 6, 7);
    disp();
    insert(8, 7, 6, 6);
    disp();
    rem();
    disp();
    rem();
    disp();
    rem();
    disp();
    printf("%d\n",isqempty());*/
    /////////////////////////

    return 0;
}