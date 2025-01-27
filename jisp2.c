#include <stdlib.h>
#include <stdio.h>
#include "jisp2_solver.h"

void jisp2(int argc, char* argv[])
{
    FILE *fin;
    int is_input_file = 0;
    if (argc > 1)
    {
        fin = fopen(argv[1], "r");
        is_input_file = 1;
    }

    int job_count = 0;
    if (is_input_file == 1)
    {
        fscanf(fin, "%d", &job_count);
    }
    else
    {
        scanf("%d", &job_count);
    }

    int* job_interval_endpoints =(int*)malloc(4 * job_count * sizeof(int));

    for (int i = 0; i < job_count; i++)
    {
        int first_interval_start;
        int first_interval_finish;
        int second_interval_start;
        int second_interval_finish;

        if (is_input_file == 1)
        {
            fscanf(fin, " %d %d %d %d", &first_interval_start, &first_interval_finish,
                                        &second_interval_start, &second_interval_finish);
        }
        else
        {
            scanf(" %d %d %d %d", &first_interval_start, &first_interval_finish,
                                  &second_interval_start, &second_interval_finish);
        }

        int index = i * 4;
        job_interval_endpoints[index] = first_interval_start;
        job_interval_endpoints[index + 1] = first_interval_finish;
        job_interval_endpoints[index + 2] = second_interval_start;
        job_interval_endpoints[index + 3] = second_interval_finish;
    }

    if (is_input_file == 1)
    {
        fclose(fin);
    }

    int* schedule = (int*)malloc(2 * job_count * sizeof(int));
    
    int result = jisp2_solver(job_count, job_interval_endpoints, schedule);

    free(job_interval_endpoints);

    FILE *fout;
    int is_output_file = 0;
    if (argc > 2)
    {
        fout = fopen(argv[2], "w");
        is_output_file = 1;
    }

    if (result == 1)
    {
        if (is_output_file == 1)
        {
            fprintf(fout, "SCHEDULABLE");
            for (int i = 0; i < job_count; i++)
            {
                int index = i * 2;
                fprintf(fout, "\n%d %d", schedule[index], schedule[index+1]);
            }
        }
        else
        {
            printf("SCHEDULABLE");
            for (int i = 0; i < job_count; i++)
            {
                int index = i * 2;
                printf("\n%d %d", schedule[index], schedule[index+1]);
            }
        }
    }
    else
    {
        if (is_output_file == 1)
        {
            fprintf(fout, "INFEASIBLE");
        }
        else
        {
            printf("INFEASIBLE");
        }
    }

    free(schedule);

    if (is_output_file == 1)
    {
        fclose(fout);
    }
}