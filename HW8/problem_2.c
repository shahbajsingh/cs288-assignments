#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 4

unsigned long num_terms;
long double pi = 0.0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

// compute partial sum
void *compute_partial_sum(void *arg) {
    long thread_id = (long)arg;
    long double partial_sum = 0.0;
    
    for (unsigned long i = thread_id; i < num_terms; i += NUM_THREADS) {
        long double x = ((long double)i + 0.5) / num_terms;
        partial_sum += 4.0 / (1.0 + x * x);
    }
    
    pthread_mutex_lock(&mutex); // lock the mutex to protect global sum
    pi += partial_sum; // update global sum
    pthread_mutex_unlock(&mutex); // unlock the mutex so other threads can use it
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // check for command line args
        fprintf(stderr, "ERROR: Invalid arguments\n");
        return 1;
    }

    num_terms = strtoul(argv[1], NULL, 10); // get number of terms 
    int num_threads = atoi(argv[2]);

    if (num_threads <= 0) {
        fprintf(stderr, "ERROR: Invalid number of threads\n");
        return 1;
    }

    pthread_t threads[num_threads];
    
    for (long i = 0; i < num_threads; ++i) {
        pthread_create(&threads[i], NULL, compute_partial_sum, (void *)i);
    }
    
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    long double PI = 3.14159265358979323846264338327950288419716939937510;
    long double error = fabsl(pi / num_terms - PI);

    printf("Estimated pi is approximately %.22Lf, Error is %.22Lf\n", pi / num_terms, error);

    return 0;
}
