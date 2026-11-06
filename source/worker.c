
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "worker.h"

void worker_run(int simulate_failure)
{
    pid_t pid;
    int status;

    printf("\n========== WORKER PROCESS ==========\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        /* Child process: worker */

        printf("[Worker] Process started. PID: %d\n", getpid());

        if (simulate_failure)
        {
            printf("[Worker] Simulating worker failure...\n");
            exit(1);
        }

        printf("[Worker] Executing assigned job...\n");
        sleep(2);

        printf("[Worker] Job completed successfully.\n");
        exit(0);
    }
    else
    {
        /* Parent process: monitor */

        printf("[Parent] Created worker with PID: %d\n", pid);
        printf("[Parent] Waiting for worker...\n");

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return;
        }

        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);

            if (exit_code == 0)
                printf("[Parent] Worker completed successfully.\n");
            else
                printf("[Parent] Worker failed. Exit code: %d\n", exit_code);
        }
        else if (WIFSIGNALED(status))
        {
            printf("[Parent] Worker terminated by signal: %d\n",
                   WTERMSIG(status));
        }
    }

    printf("====================================\n");
}
