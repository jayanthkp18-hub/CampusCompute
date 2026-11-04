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

void round_robin_scheduling(Job jobs[], int n, int quantum)
{
    int current_time = 0;
    int completed = 0;
    int next_job = 0;

    int queue[MAX_JOBS];
    int front = 0, rear = 0, count = 0;

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

    printf("\n===== ROUND ROBIN SCHEDULING =====\n");

    while (completed < n)
    {
        // If queue is empty, move to next job's arrival time
        if (count == 0 && next_job < n)
        {
            if (current_time < jobs[next_job].arrival_time)
                current_time = jobs[next_job].arrival_time;
        }

        // Add arrived jobs to ready queue
        while (next_job < n &&
               jobs[next_job].arrival_time <= current_time)
        {
            queue[rear] = next_job;
            rear = (rear + 1) % MAX_JOBS;
            count++;
            next_job++;
        }

        // Get next job from queue
        int index = queue[front];
        front = (front + 1) % MAX_JOBS;
        count--;

        jobs[index].status = RUNNING;

        int execution_time;

        if (jobs[index].remaining_time > quantum)
            execution_time = quantum;
        else
            execution_time = jobs[index].remaining_time;

        // Execute job
        current_time += execution_time;
        jobs[index].remaining_time -= execution_time;

        printf("Job %d | Executed: %d | Remaining: %d\n",
               jobs[index].job_id,
               execution_time,
               jobs[index].remaining_time);

        // Add newly arrived jobs
        while (next_job < n &&
               jobs[next_job].arrival_time <= current_time)
        {
            queue[rear] = next_job;
            rear = (rear + 1) % MAX_JOBS;
            count++;
            next_job++;
        }

        // Check whether job is completed
        if (jobs[index].remaining_time == 0)
        {
            jobs[index].completion_time = current_time;
            jobs[index].status = COMPLETED;
            completed++;
        }
        else
        {
            jobs[index].status = WAITING;

            // Put unfinished job at the back of queue
            queue[rear] = index;
            rear = (rear + 1) % MAX_JOBS;
            count++;
        }
    }

    // Calculate scheduling metrics
    for (int i = 0; i < n; i++)
    {
        int turnaround_time =
            jobs[i].completion_time - jobs[i].arrival_time;

        int waiting_time =
            turnaround_time - jobs[i].burst_time;

        total_waiting_time += waiting_time;
        total_turnaround_time += turnaround_time;

        printf("Job %d | Completion: %d | Waiting: %d | Turnaround: %d\n",
               jobs[i].job_id,
               jobs[i].completion_time,
               waiting_time,
               turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f",
           total_waiting_time / n);

    printf("\nAverage Turnaround Time: %.2f\n",
           total_turnaround_time / n);
}