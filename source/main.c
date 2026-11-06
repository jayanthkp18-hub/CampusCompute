
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "job.h"
#include "scheduler.h"
#include "banker.h"
#include "worker.h"
#include "resource_limits.h"
#include "logger.h"
#include "ipc.h"

/* Returns 1 when an integer is read, 0 when input reaches EOF. */
int read_integer(const char *message, int *result)
{
    char input[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", message);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            return 0;

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

        *result = (int)value;
        return 1;
    }
}

/* Clear any remaining characters after scanf-based input. */
void discard_input_line(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Discard remaining input */
    }
}

int main(void)
{
    Job jobs[MAX_JOBS];
    int n;

    printf("========================================\n");
    printf("          CAMPUSCOMPUTE\n");
    printf(" Intelligent Job Scheduling and Resource Management\n");
    printf("========================================\n");

    /* Read number of jobs */
    while (1)
    {
        if (!read_integer("\nEnter number of jobs (1-100): ", &n))
        {
            printf("\nInput ended. Exiting CampusCompute.\n");
            return 0;
        }

        if (n >= 1 && n <= MAX_JOBS)
            break;

        printf("Number of jobs must be between 1 and %d.\n", MAX_JOBS);
    }

    /* Read job details */
    for (int i = 0; i < n; i++)
    {
        int arrival, burst, priority;

        printf("\nEnter details for Job %d:\n", i + 1);

        while (1)
        {
            if (!read_integer("Arrival Time: ", &arrival))
            {
                printf("\nInput ended. Exiting CampusCompute.\n");
                return 0;
            }

            if (arrival >= 0)
                break;

            printf("Arrival time cannot be negative.\n");
        }

        while (1)
        {
            if (!read_integer("CPU Burst Time: ", &burst))
            {
                printf("\nInput ended. Exiting CampusCompute.\n");
                return 0;
            }

            if (burst > 0)
                break;

            printf("Burst time must be positive.\n");
        }

        while (1)
        {
            if (!read_integer("Priority: ", &priority))
            {
                printf("\nInput ended. Exiting CampusCompute.\n");
                return 0;
            }

            if (priority > 0)
                break;

            printf("Priority must be positive.\n");
        }

        initialize_job(&jobs[i], i + 1, arrival, burst, priority);
    }

    int choice;

    do
    {
        printf("\n========== CAMPUSCOMPUTE MENU ==========\n");
        printf("1.  FCFS Scheduling\n");
        printf("2.  Round Robin Scheduling\n");
        printf("3.  Priority Scheduling\n");
        printf("4.  Compare All Scheduling Algorithms\n");
        printf("----------------------------------------\n");
        printf("5.  Banker's Algorithm\n");
        printf("6.  Worker Process - Normal Execution\n");
        printf("7.  Worker Process - Simulate Failure\n");
        printf("8.  Resource Limit Demonstration\n");
        printf("9.  Concurrent Activity Logger\n");
        printf("10. IPC - Shared Memory and Semaphore\n");
        printf("----------------------------------------\n");
        printf("11. Exit\n");
        printf("========================================\n");

        if (!read_integer("\nEnter your choice: ", &choice))
        {
            printf("\nInput ended. Exiting CampusCompute.\n");
            break;
        }

        /* Fresh copies for scheduling algorithms */
        Job fcfs_jobs[MAX_JOBS];
        Job rr_jobs[MAX_JOBS];
        Job priority_jobs[MAX_JOBS];

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

                while (1)
                {
                    if (!read_integer("Enter Time Quantum: ", &quantum))
                    {
                        printf("\nInput ended. Exiting CampusCompute.\n");
                        return 0;
                    }

                    if (quantum > 0)
                        break;

                    printf("Time Quantum must be positive.\n");
                }

                round_robin_scheduling(rr_jobs, n, quantum);
                break;
            }

            case 3:
                priority_scheduling(priority_jobs, n);
                break;

            case 4:
            {
                /* Comparison uses a fixed Round Robin quantum of 2 */
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
                banker_run();
                discard_input_line();
                break;

            case 6:
                worker_run(0);
                break;

            case 7:
                worker_run(1);
                break;

            case 8:
                resource_limits_run();
                break;

            case 9:
                logger_run();
                break;

            case 10:
                ipc_run();
                break;

            case 11:
                printf("\nExiting CampusCompute. Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice! Please select 1-11.\n");
        }

    } while (choice != 11);

    return 0;
}
