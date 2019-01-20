#include <stdio.h>
#include <stdlib.h>

int pq[INT16_MAX][4];
int front;
int rear;

void create(){
    front = -1;
    rear = -1;
    return;
}

void insert(int n1, int n2, int n3, int p){
    printf("Insert called for %d %d %d %d\n", n1, n2, n3, p);
    if (front == -1 && rear == -1) {
        front = 0;
        rear += 1;
        printf("If : Inserting\n");
        pq[rear][0] = n1;
        pq[rear][1] = n2;
        pq[rear][2] = n3;
        pq[rear][3] = p;
        return;
    }
    else{
        int i;
        i = rear;
        int j = 0;
        printf("Else : \n");
        printf("i %d\n",i);
        while (p > pq[i][3] && i >= 0) {
            printf("In while Inserting\n");
            pq[i+1][0] = pq[i][0];
            pq[i+1][1] = pq[i][1];
            pq[i+1][2] = pq[i][2];
            pq[i+1][3] = pq[i][3];
            printf("%d %d\n", pq[i][0], pq[i+1][0]);
            printf("%d %d\n", pq[i][1], pq[i+1][1]);
            printf("%d %d\n", pq[i][2], pq[i+1][2]);
            printf("%d %d\n", pq[i][3], pq[i+1][3]);
            i -= 1;
        }
        i+=1;
        pq[i][0] = n1;
        pq[i][1] = n2;
        pq[i][2] = n3;
        pq[i][3] = p;
        printf("i %d\n",i);
        printf("%d %d\n", pq[i][0], n1);
        printf("%d %d\n", pq[i][1], n2);
        printf("%d %d\n", pq[i][2], n3);
        printf("%d %d\n", pq[i][3], p);
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
    return;
}

void disp(){
    int i = 0;
    if(front == -1 && rear == -1){
        printf("Queue is empty!!\n");
        return;
    }
    else{
        front = 0;
        int i = 0;
        printf("Front %d Rear %d\n", front, rear);
        while(i <= rear){
            printf("%d %d %d %d -> ", pq[i][0], pq[i][1], pq[i][2], pq[i][3]);
            i += 1;
        }
        printf("\n");
        return;
    }
}

int main(){

    create();
    disp();
    insert(1, 2, 3, 4);
    disp();
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
    return 0;
}