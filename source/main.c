
#include <stdio.h>
#include "job.h"

int main()
{
    Job jobs[3];

    initialize_job(&jobs[0], 1, 0, 5, 2);
    initialize_job(&jobs[1], 2, 1, 3, 1);
    initialize_job(&jobs[2], 3, 2, 8, 3);

    printf("===== CampusCompute =====\n\n");

    for (int i = 0; i < 3; i++)
    {
        display_job(jobs[i]);
        printf("----------------------\n");
    }

    return 0;
}
