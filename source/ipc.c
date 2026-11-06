#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>
#include <errno.h>

#include "ipc.h"

#define SHM_NAME "/campuscompute_shared_memory"
#define SEM_NAME "/campuscompute_semaphore"
#define MESSAGE_SIZE 256

typedef struct
{
    char message[MESSAGE_SIZE];
} SharedData;

void ipc_run(void)
{
    int shm_fd = -1;
    SharedData *shared_data = MAP_FAILED;
    sem_t *semaphore = SEM_FAILED;

    pid_t pid = -1;
    int status;
    int shm_created = 0;
    int sem_created = 0;
    int child_created = 0;

    printf("\n========== IPC DEMONSTRATION ==========\n");

    /* Create shared memory */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (shm_fd == -1)
    {
        perror("shm_open failed (object may already exist)");
        goto cleanup;
    }

    shm_created = 1;

    if (ftruncate(shm_fd, sizeof(SharedData)) == -1)
    {
        perror("ftruncate failed");
        goto cleanup;
    }

    shared_data = mmap(NULL, sizeof(SharedData),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED, shm_fd, 0);

    if (shared_data == MAP_FAILED)
    {
        perror("mmap failed");
        goto cleanup;
    }

    /* Create named semaphore */
    semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0600, 0);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open failed");
        goto cleanup;
    }

    sem_created = 1;

    /* Create child process */
    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        goto cleanup;
    }

    if (pid == 0)
    {
        /* Child process: receiver */
        printf("[Child] Waiting for message...\n");
        fflush(stdout);

        int wait_result;

        do
        {
            wait_result = sem_wait(semaphore);
        }
        while (wait_result == -1 && errno == EINTR);

        if (wait_result == -1)
        {
            perror("[Child] sem_wait failed");

            sem_close(semaphore);
            munmap(shared_data, sizeof(SharedData));
            close(shm_fd);

            _exit(EXIT_FAILURE);
        }

        printf("[Child] Received message: %s\n",
               shared_data->message);

        sem_close(semaphore);
        munmap(shared_data, sizeof(SharedData));
        close(shm_fd);

        _exit(EXIT_SUCCESS);
    }

    child_created = 1;

    /* Parent process: sender */
    printf("[Parent] Created shared memory and semaphore.\n");

    snprintf(shared_data->message, MESSAGE_SIZE,
             "Hello from Parent Process!");

    printf("[Parent] Writing message to shared memory.\n");

    if (sem_post(semaphore) == -1)
    {
        perror("sem_post failed");
        kill(pid, SIGTERM);
    }

    /* Wait for child process */
    pid_t wait_result;

    do
    {
        wait_result = waitpid(pid, &status, 0);
    }
    while (wait_result == -1 && errno == EINTR);

    if (wait_result == -1)
    {
        perror("waitpid failed");
    }
    else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        printf("[Parent] Child received the message successfully.\n");
    }
    else
    {
        printf("[Parent] Child process failed.\n");
    }

cleanup:

    if (child_created == 0 && pid > 0)
    {
        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
    }

    if (semaphore != SEM_FAILED)
        sem_close(semaphore);

    if (shared_data != MAP_FAILED)
        munmap(shared_data, sizeof(SharedData));

    if (shm_fd != -1)
        close(shm_fd);

    if (sem_created && sem_unlink(SEM_NAME) == -1)
        perror("sem_unlink failed");

    if (shm_created && shm_unlink(SHM_NAME) == -1)
        perror("shm_unlink failed");

    printf("[IPC] Shared memory and semaphore cleaned up.\n");
    printf("=======================================\n");
}