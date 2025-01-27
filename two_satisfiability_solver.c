#include <stdlib.h>
#include "twosat.h"

int two_satisfiability_solver(int variable_count, int disjunction_count, int* disjunctions, int* model)
{
    int double_variable_count = variable_count + variable_count; 
    int implication_graph_size = double_variable_count*double_variable_count;
    int* implication_graph = (int*)malloc(implication_graph_size * sizeof(int));

    for(int i = 0; i < double_variable_count; i++)
    {
        for(int j = 0; j < double_variable_count; j++)
        {
            implication_graph[i * double_variable_count + j] = 0;
        }
    }

    for (int i = 0; i < disjunction_count; i++)
    {
        int first_disjunct = disjunctions[2*i];
        int second_disjunct = disjunctions[2*i + 1];

        int i1 = variable_count + first_disjunct - 1;
        int j1 = second_disjunct - 1;
        int i2 = variable_count + second_disjunct - 1;
        int j2 = first_disjunct - 1;

        if (first_disjunct > 0)
        {
            if (second_disjunct < 0)
            {
                j1 = variable_count + (-1)*second_disjunct - 1;
                i2 = (-1)*second_disjunct - 1;
            }
        }
        else
        {
            if (second_disjunct > 0)
            {
                i1 = (-1)*first_disjunct - 1;
                j2 = variable_count + (-1)*first_disjunct - 1;
            }
            else
            {
                i1 = (-1)*first_disjunct - 1;
                j1 = variable_count + (-1)*second_disjunct - 1;
                i2 = (-1)*second_disjunct - 1;
                j2 = variable_count + (-1)*first_disjunct - 1;
            }
        }

        implication_graph[i1 * double_variable_count + j1] = 1;
        implication_graph[i2 * double_variable_count + j2] = 1;
    }

    int result = twosat(variable_count, implication_graph, model);

    free(implication_graph);
    return result;
}