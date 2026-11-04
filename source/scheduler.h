#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"


typedef struct {
    float average_waiting_time;
    float average_turnaround_time;
} SchedulingMetrics;


void fcfs_scheduling(Job jobs[], int n);
void round_robin_scheduling(Job jobs[], int n, int quantum);
void priority_scheduling(Job jobs[], int n);
SchedulingMetrics calculate_metrics(Job jobs[], int n);

#endif