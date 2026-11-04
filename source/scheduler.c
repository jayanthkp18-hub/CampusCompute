#include <stdio.h>
#include "scheduler.h"

void fcfs_scheduling(Job jobs[], int n)
{
    int current_time = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    // Sort jobs by arrival time
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (jobs[j].arrival_time > jobs[j + 1].arrival_time)
            {
                Job temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }

    printf("\n===== FCFS SCHEDULING =====\n");

    for (int i = 0; i < n; i++)
    {
        if (current_time < jobs[i].arrival_time)
            current_time = jobs[i].arrival_time;

        jobs[i].status = RUNNING;

        int waiting_time = current_time - jobs[i].arrival_time;

        current_time += jobs[i].burst_time;

        jobs[i].completion_time = current_time;
        jobs[i].remaining_time = 0;
        jobs[i].status = COMPLETED;

        int turnaround_time =
            jobs[i].completion_time - jobs[i].arrival_time;

        total_waiting_time += waiting_time;
        total_turnaround_time += turnaround_time;

        printf("Job %d | Start: %d | Completion: %d | Waiting: %d | Turnaround: %d\n",
               jobs[i].job_id,
               current_time - jobs[i].burst_time,
               jobs[i].completion_time,
               waiting_time,
               turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f",
           total_waiting_time / n);

    printf("\nAverage Turnaround Time: %.2f\n",
           total_turnaround_time / n);
}