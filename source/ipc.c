
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
    int shm_fd;
    SharedData *shared_data;
    sem_t *semaphore;
    pid_t pid;
    int status;

    int child_created = 0;
    int sem_created = 0;
    int shm_created = 0;

    printf("\n========== IPC DEMONSTRATION ==========\n");

    /* Create shared memory */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (shm_fd == -1)
    {
        perror("shm_open failed (object may already exist)");
        return;
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
        munmap(shared_data, sizeof(SharedData));
        goto cleanup;
    }

    sem_created = 1;

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        sem_close(semaphore);
        munmap(shared_data, sizeof(SharedData));
        goto cleanup;
    }

    if (pid == 0)
    {
        /* Child process: receiver */

        printf("[Child] Waiting for message...\n");
        fflush(stdout);

        if (sem_wait(semaphore) == -1)
        {
            perror("[Child] sem_wait failed");
            sem_close(semaphore);
            munmap(shared_data, sizeof(SharedData));
            close(shm_fd);
            exit(EXIT_FAILURE);
        }

        printf("[Child] Received message: %s\n",
               shared_data->message);

        sem_close(semaphore);
        munmap(shared_data, sizeof(SharedData));
        close(shm_fd);

        exit(EXIT_SUCCESS);
    }

    /* Parent process: sender */

    child_created = 1;

    printf("[Parent] Created shared memory and semaphore.\n");

    snprintf(shared_data->message, MESSAGE_SIZE,
             "Hello from Parent Process!");

    printf("[Parent] Writing message to shared memory.\n");

    if (sem_post(semaphore) == -1)
    {
        perror("sem_post failed");
        kill(pid, SIGTERM);
    }

    if (waitpid(pid, &status, 0) == -1)
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

    sem_close(semaphore);
    munmap(shared_data, sizeof(SharedData));

cleanup:

    if (child_created == 0)
    {
        /* No child is using these objects */
    }

    if (sem_created)
        sem_unlink(SEM_NAME);

    if (shm_created)
        shm_unlink(SHM_NAME);

    close(shm_fd);

    printf("[IPC] Shared memory and semaphore cleaned up.\n");
    printf("=======================================\n");
}
