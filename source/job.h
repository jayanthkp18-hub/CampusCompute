#ifndef JOB_H
#define JOB_H

#define MAX_JOBS 100

typedef enum
{
    WAITING,
    RUNNING,
    COMPLETED
} JobStatus;

typedef struct
{
    int job_id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    JobStatus status;
} Job;

void initialize_job(Job *job, int id, int arrival,
                    int burst, int priority);

void display_job(Job job);

#endif