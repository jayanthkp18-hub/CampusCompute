
#ifndef BANKER_H
#define BANKER_H

#define BANKER_MAX_PROCESSES 10
#define BANKER_MAX_RESOURCES 10

/*
 * Return values:
 *  1  = System is safe
 *  0  = System is unsafe
 * -1  = Invalid input
 */

int banker_check_safety(
    int processes,
    int resources,
    const int available[],
    const int allocation[][BANKER_MAX_RESOURCES],
    const int maximum[][BANKER_MAX_RESOURCES],
    int safe_sequence[]
);

/* Interactive interface for testing the Banker's Algorithm */
void banker_run(void);

#endif
