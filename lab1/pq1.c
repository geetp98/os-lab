#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    int i = rear;
    front = 0;

    while (i > 0) {
        pq[i-1][0] = pq[i][0];
        pq[i-1][1] = pq[i][1];
        pq[i-1][2] = pq[i][2];
        pq[i-1][3] = pq[i][3];
        i-=1;
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

int main(){

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
    printf("%d\n",isqempty());
    return 0;
}