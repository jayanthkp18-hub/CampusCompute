
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

    int choice;

    do
    {
        printf("\n===== CAMPUSCOMPUTE MENU =====\n");
        printf("1. FCFS Scheduling\n");
        printf("2. Round Robin Scheduling\n");
        printf("3. Priority Scheduling\n");
        printf("4. Compare All Algorithms\n");
        printf("5. Exit\n");

        choice = read_integer("\nEnter your choice: ");

        Job fcfs_jobs[MAX_JOBS];
        Job rr_jobs[MAX_JOBS];
        Job priority_jobs[MAX_JOBS];

        // Create fresh copies
        for (int i = 0; i < n; i++)
        {
            fcfs_jobs[i] = jobs[i];
            rr_jobs[i] = jobs[i];
            priority_jobs[i] = jobs[i];
        }

        switch (choice)
        {
            case 1:
                fcfs_scheduling(fcfs_jobs, n);
                break;

            case 2:
            {
                int quantum;

                do
                {
                    quantum = read_integer("Enter Time Quantum: ");

                    if (quantum <= 0)
                        printf("Time Quantum must be positive.\n");

                } while (quantum <= 0);

                round_robin_scheduling(rr_jobs, n, quantum);
                break;
            }

            case 3:
                priority_scheduling(priority_jobs, n);
                break;

            case 4:
            {
                fcfs_scheduling(fcfs_jobs, n);
                round_robin_scheduling(rr_jobs, n, 2);
                priority_scheduling(priority_jobs, n);

                SchedulingMetrics fcfs_metrics =
                    calculate_metrics(fcfs_jobs, n);

                SchedulingMetrics rr_metrics =
                    calculate_metrics(rr_jobs, n);

                SchedulingMetrics priority_metrics =
                    calculate_metrics(priority_jobs, n);

                printf("\n========== SCHEDULING COMPARISON ==========\n");

                printf("%-20s %-20s %-20s\n",
                       "Algorithm", "Avg Waiting", "Avg Turnaround");

                printf("------------------------------------------------------------\n");

                printf("%-20s %-20.2f %-20.2f\n",
                       "FCFS",
                       fcfs_metrics.average_waiting_time,
                       fcfs_metrics.average_turnaround_time);

                printf("%-20s %-20.2f %-20.2f\n",
                       "Round Robin",
                       rr_metrics.average_waiting_time,
                       rr_metrics.average_turnaround_time);

                printf("%-20s %-20.2f %-20.2f\n",
                       "Priority",
                       priority_metrics.average_waiting_time,
                       priority_metrics.average_turnaround_time);

                break;
            }

            case 5:
                printf("\nExiting CampusCompute. Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice! Please select 1-5.\n");
        }

    } while (choice != 5);

    return 0;
}
