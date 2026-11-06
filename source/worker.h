
#ifndef WORKER_H
#define WORKER_H

/*
 * Runs a worker process.
 * simulate_failure = 0 : Normal execution
 * simulate_failure = 1 : Simulated failure
 */
void worker_run(int simulate_failure);

#endif
