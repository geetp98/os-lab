#include <stdio.h>
#include <stdlib.h>

#define MAX 5

void insert_by_priority(int);
void delete_by_priority(int);
void create();
void check(int);
void display_pqueue();

int pq_pid[MAX];
int pq_artime[MAX];
int pq_brtime[MAX];

int front, rear;

int main()
{
    int n1, n2, n3, ch;

    printf("\n1 - Insert an element into queue");
    printf("\n2 - Delete an element from queue");
    printf("\n3 - Display queue elements");
    printf("\n4 - Exit");

    create();

    while (1)
    {
        printf("\nEnter your choice : ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            printf("\nEnter value to be inserted : ");
            scanf("%d %d %d", &n1, &n2, &n3);
            int p = n2;
            insert_by_priority(n1, n2, n3, p);
            break;
        case 2:
            printf("\nEnter value to delete : ");
            scanf("%d", &n);
            delete_by_priority(n);
            break;
        case 3:
            display_pqueue();
            break;
        case 4:
            exit(0);
        default:
            printf("\nChoice is incorrect, Enter a correct choice");
        }
    }
    return 0;
}

/* Function to create an empty priority queue */
void create()
{
    front = rear = -1;
}

/* Function to insert value into priority queue */
void insert_by_priority(int data)
{
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        pq_pid[rear] = data;
        return;
    }
    else
        check(data);
    rear++;
}

/* Function to check priority and place element */
void check(int data)
{
    int i, j;

    for (i = 0; i <= rear; i++)
    {
        if (data >= pq_pid[i])
        {
            for (j = rear + 1; j > i; j--)
            {
                pq_pid[j] = pq_pid[j - 1];
            }
            pq_pid[i] = data;
            return;
        }
    }
    pq_pid[i] = data;
}

/* Function to delete an element from queue */
void delete_by_priority(int data)
{
    int i;

    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }

    for (i = 0; i <= rear; i++)
    {
        if (data == pq_pid[i])
        {
            for (; i < rear; i++)
            {
                pq_pid[i] = pq_pid[i + 1];
            }

            pq_pid[i] = -99;
            rear--;

            if (rear == -1)
                front = -1;
            return;
        }
    }
    printf("\n%d not found in queue to delete", data);
}

/* Function to display queue elements */
void display_pqueue()
{
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++)
    {
        printf(" %d ", pq_pid[front]);
    }

    front = 0;
}