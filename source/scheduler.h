#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"

void fcfs_scheduling(Job jobs[], int n);
void round_robin_scheduling(Job jobs[], int n, int quantum);

#endif