
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>

#include "resource_limits.h"

void resource_limits_run(void)
{
    pid_t pid;
    int status;

    struct rlimit limit;

    printf("\n========== RESOURCE LIMITS ==========\n");

    /* Set CPU time limit to 2 seconds */
    limit.rlim_cur = 2;
    limit.rlim_max = 2;

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        /* Child process */

        if (setrlimit(RLIMIT_CPU, &limit) == -1)
        {
            perror("setrlimit failed");
            exit(EXIT_FAILURE);
        }

        printf("[Worker] CPU time limit set to 2 seconds.\n");
        printf("[Worker] Starting CPU-intensive task...\n");
        fflush(stdout);

        volatile unsigned long long count = 0;

        while (1)
        {
            count++;
        }
    }
    else
    {
        /* Parent process */

        printf("[Parent] Monitoring worker PID: %d\n", pid);

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return;
        }

        if (WIFSIGNALED(status))
        {
            int signal_number = WTERMSIG(status);

            if (signal_number == SIGXCPU || signal_number == SIGKILL)
                printf("[Parent] Worker terminated after exceeding CPU time limit.\n");
            else
                printf("[Parent] Worker terminated by signal %d.\n",
                       signal_number);
        }
        else if (WIFEXITED(status))
        {
            printf("[Parent] Worker exited with status %d.\n",
                   WEXITSTATUS(status));
        }
    }

    printf("=====================================\n");
}
