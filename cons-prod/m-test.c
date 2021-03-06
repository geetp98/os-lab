#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/****
 *
 * The program takes an integer command-line argument, N.  The program creates
 * a thread to print consecutive odd numbers from 1 to N.  The main thread
 * prints consecutive even integers from 2 to N.
 *
 * The main thread and created thread run asynchronously. This means that 
 * there is the same form of race condition between the threads, such that 
 * standard output of odd and even numbers is unpredictably interspersed.
 *
 */

/**
 * Print consecutive odd integers from 1 to arg.  The signature of this
 * function is that required for a thread start routine.
 */
 
 pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
 
 pthread_cond_t c = PTHREAD_COND_INITIALIZER;
 
 int flag;
 
void* print_odds(void* arg) {
   int i;
   int n = *((int*) arg);

   for (i = 1; i <= n; i += 2)
   {
	   pthread_mutex_lock(&m);
	   while(flag == 0){
		   pthread_cond_wait(&c, &m);
		}
      printf("%d\n", i);
      flag=0;
      pthread_cond_signal(&c);
      pthread_mutex_unlock(&m);
   }

   return NULL;
}

/*
 * Print consecutive odd and even integers, up to n.  A created thread prints
 * the odds, the main thread prints the evens.
 */
void print_numbers(int n)
{
    int i;		/* loop index */
    pthread_t tid;	/* id for thread to be created */
    int *arg;		/* argument sent to thread start routine */

    /*
     * Allocate storage for the start routine argument, which must be void*.
     */
    if ((arg = malloc(sizeof(int))) == NULL) {
       perror("malloc");
       exit(-1);
    }

    /*
     * Initialize the argument.
     */
    *arg = n;

    /*
     * Create the thread, which means its start routine begins quasi-concurrent
     * execution.
     */
    if (pthread_create(&tid, NULL, print_odds, arg)) {
       perror("pthread_create");
       exit(-1);
    }

    /*
     * Back in the main thread, print out even integers, concurrently with the
     * odd printing in the thread.
     */
     flag = 1;
    for (i = 2; i <= n; i += 2) {
		pthread_mutex_lock(&m);
	   while(flag == 1){
		   pthread_cond_wait(&c, &m);
		}
      printf("\t%d\n", i);
      flag=1;
      pthread_cond_signal(&c);
      pthread_mutex_unlock(&m);
    }

    /*
     * Wait for the thread to terminate.
     */
    if (pthread_join(tid, NULL)) {
       perror("pthread_join");
       exit(-1);
    }
}


/*
 * Get the single command-line argument and call print_numbers with it.
 */
int main(int argc, char *argv[]) {

    int n;		/* integer value of command-line arg */
    char* end;		/* for use with strtol */

    /*
     * Make sure there is exactly one command-line arg.
     */
    if (argc != 2) {
       fprintf(stderr, "usage: %s <number>\n", argv[0]);
       exit(1);
    }

    /*
     * Convert and validate arg as an integer.
     */
    n = strtol(argv[1], &end, 10);
    if (*end != '\0') {
       fprintf(stderr, "%s - %s is not a valid integer\n", argv[0], argv[1]);
       exit(1);
    }

    /*
     * Do the deed.
     */
    print_numbers(n);

    return EXIT_SUCCESS;
}
