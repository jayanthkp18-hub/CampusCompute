
#include <stdio.h>
#include "job.h"

void initialize_job(Job *job, int id, int arrival,
                    int burst, int priority)
{
    job->job_id = id;
    job->arrival_time = arrival;
    job->burst_time = burst;
    job->priority = priority;

    job->remaining_time = burst;
    job->completion_time = 0;
    job->status = WAITING;
}

void display_job(Job job)
{
    printf("Job ID: J%d\n", job.job_id);
    printf("Arrival Time: %d\n", job.arrival_time);
    printf("Burst Time: %d\n", job.burst_time);
    printf("Priority: %d\n", job.priority);
    printf("Status: %d\n", job.status);
}
