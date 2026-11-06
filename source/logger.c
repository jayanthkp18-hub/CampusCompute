#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "logger.h"

#define QUEUE_SIZE 5
#define MESSAGE_SIZE 256
#define TOTAL_MESSAGES 10

typedef struct
{
    char messages[QUEUE_SIZE][MESSAGE_SIZE];
    int front;
    int rear;
    int count;
    int finished;
    int failed;
} LogQueue;

static LogQueue queue = {0};

static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

static int enqueue(const char *message)
{
    pthread_mutex_lock(&queue_mutex);

    while (queue.count == QUEUE_SIZE &&
           !queue.finished && !queue.failed)
    {
        pthread_cond_wait(&not_full, &queue_mutex);
    }

    if (queue.finished || queue.failed)
    {
        pthread_mutex_unlock(&queue_mutex);
        return 0;
    }

    snprintf(queue.messages[queue.rear], MESSAGE_SIZE, "%s", message);

    queue.rear = (queue.rear + 1) % QUEUE_SIZE;
    queue.count++;

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&queue_mutex);

    return 1;
}

static int dequeue(char *message)
{
    pthread_mutex_lock(&queue_mutex);

    while (queue.count == 0 && !queue.finished && !queue.failed)
    {
        pthread_cond_wait(&not_empty, &queue_mutex);
    }

    if (queue.failed || (queue.count == 0 && queue.finished))
    {
        pthread_mutex_unlock(&queue_mutex);
        return 0;
    }

    snprintf(message, MESSAGE_SIZE, "%s", queue.messages[queue.front]);

    queue.front = (queue.front + 1) % QUEUE_SIZE;
    queue.count--;

    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&queue_mutex);

    return 1;
}

static void *producer(void *arg)
{
    (void)arg;

    for (int i = 1; i <= TOTAL_MESSAGES; i++)
    {
        char message[MESSAGE_SIZE];

        snprintf(message, MESSAGE_SIZE,
                 "Job activity %d generated", i);

        if (!enqueue(message))
            break;
    }

    pthread_mutex_lock(&queue_mutex);
    queue.finished = 1;
    pthread_cond_broadcast(&not_empty);
    pthread_cond_broadcast(&not_full);
    pthread_mutex_unlock(&queue_mutex);

    return NULL;
}

static void *consumer(void *arg)
{
    (void)arg;

    FILE *log_file = fopen("campuscompute.log", "a");

    if (log_file == NULL)
    {
        perror("Unable to open log file");

        pthread_mutex_lock(&queue_mutex);
        queue.failed = 1;
        queue.finished = 1;
        pthread_cond_broadcast(&not_full);
        pthread_cond_broadcast(&not_empty);
        pthread_mutex_unlock(&queue_mutex);

        return NULL;
    }

    char message[MESSAGE_SIZE];

    while (dequeue(message))
    {
        printf("[Logger] %s\n", message);
        fprintf(log_file, "[Logger] %s\n", message);
        fflush(log_file);
    }

    fclose(log_file);
    return NULL;
}

void logger_run(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    printf("\n========== ACTIVITY LOGGER ==========\n");

    pthread_mutex_lock(&queue_mutex);

    queue.front = 0;
    queue.rear = 0;
    queue.count = 0;
    queue.finished = 0;
    queue.failed = 0;

    pthread_mutex_unlock(&queue_mutex);

    int result = pthread_create(&consumer_thread, NULL, consumer, NULL);

    if (result != 0)
    {
        fprintf(stderr, "Failed to create consumer thread: %d\n", result);
        return;
    }

    result = pthread_create(&producer_thread, NULL, producer, NULL);

    if (result != 0)
    {
        fprintf(stderr, "Failed to create producer thread: %d\n", result);

        pthread_mutex_lock(&queue_mutex);
        queue.finished = 1;
        pthread_cond_broadcast(&not_empty);
        pthread_cond_broadcast(&not_full);
        pthread_mutex_unlock(&queue_mutex);

        pthread_join(consumer_thread, NULL);
        return;
    }

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    if (queue.failed)
    {
        printf("[Logger] Logging stopped because the log file could not be opened.\n");
    }
    else
    {
        printf("[Logger] All messages processed.\n");
        printf("[Logger] Threads terminated successfully.\n");
    }

    printf("=====================================\n");
}