#include <stdio.h>
#include "banker.h"

/* Clear the remaining characters from the current input line. */
static void clear_input_line(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Discard input */
    }
}

/* Read an integer safely. Returns 1 on success, 0 on failure or EOF. */
static int read_integer(const char *prompt, int *value)
{
    printf("%s", prompt);

    if (scanf("%d", value) != 1)
    {
        printf("Invalid input. Please enter an integer.\n");
        clear_input_line();
        return 0;
    }

    return 1;
}

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

    /* Banker's Safety Algorithm */
    while (completed < processes)
    {
        int found = 0;

        for (int i = 0; i < processes; i++)
        {
            if (finish[i] == 0)
            {
                int can_finish = 1;

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

        if (found == 0)
            return 0;
    }

    return 1;
}

/* Interactive interface for Banker's Algorithm. */
void banker_run(void)
{
    int processes, resources;

    int available[BANKER_MAX_RESOURCES];

    int allocation[BANKER_MAX_PROCESSES]
                  [BANKER_MAX_RESOURCES] = {0};

    int maximum[BANKER_MAX_PROCESSES]
               [BANKER_MAX_RESOURCES] = {0};

    int safe_sequence[BANKER_MAX_PROCESSES];

    char prompt[100];

    printf("\n========== BANKER'S ALGORITHM ==========\n");

    if (!read_integer("Enter number of processes: ", &processes))
        return;

    if (processes < 1 || processes > BANKER_MAX_PROCESSES)
    {
        printf("Invalid number of processes. Allowed range: 1-%d.\n",
               BANKER_MAX_PROCESSES);
        return;
    }

    if (!read_integer("Enter number of resource types: ", &resources))
        return;

    if (resources < 1 || resources > BANKER_MAX_RESOURCES)
    {
        printf("Invalid number of resource types. Allowed range: 1-%d.\n",
               BANKER_MAX_RESOURCES);
        return;
    }

    /* Read Available resources */
    printf("\nEnter Available resources:\n");

    for (int j = 0; j < resources; j++)
    {
        snprintf(prompt, sizeof(prompt), "Available[%d]: ", j);

        if (!read_integer(prompt, &available[j]))
            return;

        if (available[j] < 0)
        {
            printf("Available resources cannot be negative.\n");
            return;
        }
    }

    /* Read Allocation matrix */
    printf("\nEnter Allocation matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        printf("Process P%d:\n", i);

        for (int j = 0; j < resources; j++)
        {
            snprintf(prompt, sizeof(prompt),
                     "Allocation[P%d][R%d]: ", i, j);

            if (!read_integer(prompt, &allocation[i][j]))
                return;

            if (allocation[i][j] < 0)
            {
                printf("Allocation values cannot be negative.\n");
                return;
            }
        }
    }

    /* Read Maximum matrix */
    printf("\nEnter Maximum matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        printf("Process P%d:\n", i);

        for (int j = 0; j < resources; j++)
        {
            snprintf(prompt, sizeof(prompt),
                     "Maximum[P%d][R%d]: ", i, j);

            if (!read_integer(prompt, &maximum[i][j]))
                return;

            if (maximum[i][j] < allocation[i][j])
            {
                printf("Maximum cannot be less than Allocation.\n");
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