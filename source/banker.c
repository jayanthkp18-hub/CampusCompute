
#include <stdio.h>
#include "banker.h"

/*
 * Checks whether the current system state is safe.
 *
 * Returns:
 *  1  -> Safe state
 *  0  -> Unsafe state
 * -1  -> Invalid input
 */
int banker_check_safety(
    int processes,
    int resources,
    const int available[],
    const int allocation[][BANKER_MAX_RESOURCES],
    const int maximum[][BANKER_MAX_RESOURCES],
    int safe_sequence[]
)
{
    int need[BANKER_MAX_PROCESSES][BANKER_MAX_RESOURCES];
    int work[BANKER_MAX_RESOURCES];
    int finish[BANKER_MAX_PROCESSES] = {0};

    int completed = 0;

    /* Validate dimensions */
    if (processes < 1 || processes > BANKER_MAX_PROCESSES ||
        resources < 1 || resources > BANKER_MAX_RESOURCES)
    {
        return -1;
    }

    /* Copy Available into Work */
    for (int j = 0; j < resources; j++)
    {
        if (available[j] < 0)
            return -1;

        work[j] = available[j];
    }

    /* Calculate Need = Maximum - Allocation */
    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            if (allocation[i][j] < 0 ||
                maximum[i][j] < 0 ||
                maximum[i][j] < allocation[i][j])
            {
                return -1;
            }

            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    /* Safety algorithm */
    while (completed < processes)
    {
        int found = 0;

        for (int i = 0; i < processes; i++)
        {
            if (finish[i] == 0)
            {
                int can_finish = 1;

                /* Check Need[i] <= Work */
                for (int j = 0; j < resources; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        can_finish = 0;
                        break;
                    }
                }

                if (can_finish)
                {
                    /* Simulate process completion */
                    for (int j = 0; j < resources; j++)
                    {
                        work[j] += allocation[i][j];
                    }

                    finish[i] = 1;
                    safe_sequence[completed] = i;

                    completed++;
                    found = 1;
                }
            }
        }

        /* No unfinished process could proceed */
        if (found == 0)
        {
            return 0;
        }
    }

    return 1;
}

/*
 * Interactive interface for testing Banker's Algorithm.
 */
void banker_run(void)
{
    int processes, resources;

    int available[BANKER_MAX_RESOURCES];

    int allocation[BANKER_MAX_PROCESSES]
                  [BANKER_MAX_RESOURCES] = {0};

    int maximum[BANKER_MAX_PROCESSES]
               [BANKER_MAX_RESOURCES] = {0};

    int safe_sequence[BANKER_MAX_PROCESSES];

    printf("\n========== BANKER'S ALGORITHM ==========\n");

    printf("Enter number of processes (1-%d): ",
           BANKER_MAX_PROCESSES);

    if (scanf("%d", &processes) != 1 ||
        processes < 1 || processes > BANKER_MAX_PROCESSES)
    {
        printf("Invalid number of processes.\n");
        return;
    }

    printf("Enter number of resource types (1-%d): ",
           BANKER_MAX_RESOURCES);

    if (scanf("%d", &resources) != 1 ||
        resources < 1 || resources > BANKER_MAX_RESOURCES)
    {
        printf("Invalid number of resource types.\n");
        return;
    }

    /* Read Available resources */
    printf("\nEnter Available resources (%d values):\n", resources);

    for (int j = 0; j < resources; j++)
    {
        if (scanf("%d", &available[j]) != 1 ||
            available[j] < 0)
        {
            printf("Invalid Available resource value.\n");
            return;
        }
    }

    /* Read Allocation matrix */
    printf("\nEnter Allocation matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        printf("P%d: ", i);

        for (int j = 0; j < resources; j++)
        {
            if (scanf("%d", &allocation[i][j]) != 1 ||
                allocation[i][j] < 0)
            {
                printf("Invalid Allocation value.\n");
                return;
            }
        }
    }

    /* Read Maximum matrix */
    printf("\nEnter Maximum matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        printf("P%d: ", i);

        for (int j = 0; j < resources; j++)
        {
            if (scanf("%d", &maximum[i][j]) != 1 ||
                maximum[i][j] < allocation[i][j])
            {
                printf("Invalid Maximum value. "
                       "Maximum must be >= Allocation.\n");
                return;
            }
        }
    }

    /* Display Need matrix */
    printf("\nNeed Matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        printf("P%d: ", i);

        for (int j = 0; j < resources; j++)
        {
            printf("%d ", maximum[i][j] - allocation[i][j]);
        }

        printf("\n");
    }

    /* Execute safety algorithm */
    int result = banker_check_safety(
        processes,
        resources,
        available,
        allocation,
        maximum,
        safe_sequence
    );

    if (result == 1)
    {
        printf("\nSystem is in a SAFE state.\n");

        printf("Safe sequence: ");

        for (int i = 0; i < processes; i++)
        {
            printf("P%d", safe_sequence[i]);

            if (i != processes - 1)
                printf(" -> ");
        }

        printf("\n");
    }
    else if (result == 0)
    {
        printf("\nSystem is in an UNSAFE state.\n");
        printf("No safe sequence exists.\n");
    }
    else
    {
        printf("\nInvalid input provided.\n");
    }

    printf("========================================\n");
}
