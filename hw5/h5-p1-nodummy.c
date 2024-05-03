#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>

#define NUM_THREADS 2
#define NUM_ENQUEUE_THREADS 1
#define NUM_DEQUEUE_THREADS 1

int times = 1;

typedef struct __node_t {
    int value;
    struct __node_t *next;
} node_t;

typedef struct __queue_t {
    node_t *head;
    node_t *tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

void Queue_Init(queue_t *q) {
    q->head = q->tail = NULL;
    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);
}

void Queue_Enqueue(queue_t *q, int value) {
    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->value = value; tmp->next = NULL;
    pthread_mutex_lock(&q->tail_lock);
    if (q->head == NULL) {
        q->head = q->tail = tmp;
    }
    else {
        q->tail->next = tmp;
        q->tail = tmp;
    }
    printf("Enqueued %d\n", value);
    pthread_mutex_unlock(&q->tail_lock);
}

int Queue_Dequeue(queue_t *q, int *value) {
    pthread_mutex_lock(&q->head_lock);
    if (q->head == NULL) {
        pthread_mutex_unlock(&q->head_lock);
        *value = -1;
        return -1;
    }
    node_t *tmp = q->head;
    *value = tmp->value;
    q->head = tmp->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }

    printf("Dequeued %d\n", value);
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0;
}

void *enqueue_thread(void *arg) {
    queue_t *q = (queue_t *)arg;
    srand(time(NULL));
    for (int i = 0; i < times / NUM_ENQUEUE_THREADS; i++) {
      int value = rand()  % 10 + 1;
      Queue_Enqueue(q, value);
    }
    return NULL;
}

void *dequeue_thread(void *arg) {
    queue_t *q = (queue_t *)arg;
    int value;
    for (int i = 0; i < times / NUM_DEQUEUE_THREADS; i++) {
      Queue_Dequeue(q, &value);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i, rc;
    queue_t q;
    Queue_Init(&q);

    // Create enqueue threads
    for (i = 0; i < NUM_ENQUEUE_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, enqueue_thread, (void *)&q);
    }
    
    // Join threads
    for (i = 0; i < NUM_ENQUEUE_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
    }

    // Create dequeue threads
    for (i = NUM_ENQUEUE_THREADS; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, dequeue_thread, (void *)&q);
    }

    // Join threads
    for (i = NUM_ENQUEUE_THREADS; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
    }

    return 0;
}