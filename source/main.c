
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "job.h"
#include "scheduler.h"

int read_integer(const char *message)
{
    char input[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error. Try again.\n");
            clearerr(stdin);
            continue;
        }

        errno = 0;
        value = strtol(input, &end, 10);

        if (end == input || errno == ERANGE ||
            value < INT_MIN || value > INT_MAX)
        {
            printf("Invalid input! Enter an integer.\n");
            continue;
        }

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r')
            end++;

        if (*end != '\0')
        {
            printf("Invalid input! Enter an integer only.\n");
            continue;
        }

        return (int)value;
    }
}

int main()
{
    Job jobs[MAX_JOBS];
    int n;

    printf("===== CampusCompute =====\n\n");

    do {
        n = read_integer("Enter number of jobs (1-100): ");

        if (n < 1 || n > MAX_JOBS)
            printf("Number of jobs must be between 1 and 100.\n");

    } while (n < 1 || n > MAX_JOBS);

    for (int i = 0; i < n; i++)
    {
        int arrival, burst, priority;

        printf("\nEnter details for Job %d:\n", i + 1);

        do {
            arrival = read_integer("Arrival Time: ");
            if (arrival < 0)
                printf("Arrival time cannot be negative.\n");
        } while (arrival < 0);

        do {
            burst = read_integer("CPU Burst Time: ");
            if (burst <= 0)
                printf("Burst time must be positive.\n");
        } while (burst <= 0);

        do {
            priority = read_integer("Priority: ");
            if (priority <= 0)
                printf("Priority must be positive.\n");
        } while (priority <= 0);

        initialize_job(&jobs[i], i + 1,
                       arrival, burst, priority);   
    }

    // Store original jobs for later display (testing purposes only)
    Job original_jobs[MAX_JOBS];
    for (int i = 0; i < n; i++)
    {
        original_jobs[i] = jobs[i];
    }




    fcfs_scheduling(jobs, n);   // -------------------------> scheduling for first come first serve

    printf("\n===== JOB DETAILS =====\n");

    for (int i = 0; i < n; i++)
    {
        display_job(jobs[i]);
        printf("----------------------\n");
    }

    return 0;
}
