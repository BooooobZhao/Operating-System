#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables
int* v;
int num_elements, num_threads;
int sum = 0;
pthread_mutex_t sum_mutex;

// Structure for thread arguments
typedef struct {
    int* v;
    int index;
    int size;
} runner_args_t;

// Thread function
void* runner(void* args) {
    runner_args_t* thread_args = (runner_args_t*) args;
    int partial_sum = 0;
    for (int i = thread_args->index; i < thread_args->index + thread_args->size; i++) {
        partial_sum += thread_args->v[i];
    }
    pthread_mutex_lock(&sum_mutex);
    sum += partial_sum;
    pthread_mutex_unlock(&sum_mutex);
    free(thread_args);
    return NULL;
}

int main(int argc, char* argv[]) {
    // Check command-line arguments
    if (argc != 3) {
        printf("Usage: %s num_elements num_threads\n", argv[0]);
        exit(0);
    }
    num_elements = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    // Allocate and initialize vector
    v = malloc(num_elements * sizeof(int));
    for (int i = 0; i < num_elements; i++) {
        v[i] = i;
    }

    // Initialize mutex
    pthread_mutex_init(&sum_mutex, NULL);

    // Create threads
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        runner_args_t* thread_args = malloc(sizeof(runner_args_t));
        thread_args->v = v;
        thread_args->index = i * (num_elements / num_threads);
        thread_args->size = (i == num_threads - 1) ? (num_elements - thread_args->index) : (num_elements / num_threads);
        pthread_create(&threads[i], NULL, runner, thread_args);
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free resources and print result
    free(v);
    pthread_mutex_destroy(&sum_mutex);
    printf("Sum = %d\n", sum);
    return 0;
}
