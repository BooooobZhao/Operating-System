#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 8
#define NUM_ENQUEUE_THREADS 4
#define NUM_DEQUEUE_THREADS 4
int entimes = 0;
int detimes = 0;
int times = 10000;

typedef struct __node_t {
    int value;
    struct __node_t *next;
} node_t;

typedef struct __queue_t {
    node_t *head;
    node_t *tail;
    //pthread_mutex_t head_lock, tail_lock;
} queue_t;

void Queue_Init(queue_t *q) {
    //Creating the dummy node, which is needed so that ¡°tail¡± update is separated from ¡°head¡± update.
    node_t *tmp = malloc(sizeof(node_t));
    tmp->next = NULL; //a dummy node has next=NULL and no need to set its value attribute
    q->head = q->tail = tmp;
    //pthread_mutex_init(&q->head_lock, NULL);
    //pthread_mutex_init(&q->tail_lock, NULL);
}

void Queue_Enqueue(queue_t *q, int value) {
    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->value = value; tmp->next = NULL;
    //pthread_mutex_lock(&q->tail_lock);
    q->tail->next = tmp;
    q->tail = tmp;// tail always points to the last enqueued node or
                  // to the dummy node (if the queue is empty)
    entimes = entimes + 1;
    //pthread_mutex_unlock(&q->tail_lock);
}

int Queue_Dequeue(queue_t *q, int *value) {
    //pthread_mutex_lock(&q->head_lock);
    node_t *tmp = q->head;
    node_t *new_head = tmp->next;
    if (new_head == NULL) {// indicates an empty queue with only a dummy node
        //pthread_mutex_unlock(&q->head_lock);
        *value = -1;
        return -1;
    }
    *value = new_head->value;
    q->head = new_head; //head always points to a dummy node,
                        //which is either the original dummy node or most recently dequeued node
    detimes = detimes + 1;
    //pthread_mutex_unlock(&q->head_lock);
    free(tmp); // old dummy node is removed
    return 0;
}

void *enqueue_thread(void *arg) {
    queue_t *q = (queue_t *)arg;
    srand(time(NULL));
    for (int i = 0; i < times / NUM_ENQUEUE_THREADS; i++) {
      int value = rand()  % 10 + 1;
      Queue_Enqueue(q, value);
    }
    //printf("Enqueued %d\n", value);
    return NULL;
}

void *dequeue_thread(void *arg) {
    queue_t *q = (queue_t *)arg;
    int value;
    for (int i = 0; i < times / NUM_DEQUEUE_THREADS; i++) {
      Queue_Dequeue(q, &value);
    }
    //printf("Dequeued %d\n", value);
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
    
    printf("Enqueued %d times, it should be %d.\n", entimes, times);
    printf("dequeued %d times, it should be %d.\n", detimes, times);
    
    return 0;
}