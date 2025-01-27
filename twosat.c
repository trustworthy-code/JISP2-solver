#include <stdlib.h>
#include "twosat_solver.h"
#include "twosat_solution.h"

int twosat(int variable_count, int* implication_graph, int* model)
{
    int double_variable_count = variable_count + variable_count; 

    for(int k = 0; k < double_variable_count; k++)
    {
        for(int i = 0; i < double_variable_count; i++)
        {
            for(int j = 0; j < double_variable_count; j++)
            {
                if ((implication_graph[i * double_variable_count + j] == 0) &&
                    (implication_graph[i * double_variable_count + k] == 1) &&
                    (implication_graph[k * double_variable_count + j] == 1))
                {
                    implication_graph[i * double_variable_count + j] = 1;
                }
            }
        }
    }

    int result = twosat_solver(variable_count, implication_graph);

    if (result == 1)
    {
        int* strongly_connected_components = (int*)malloc(double_variable_count * sizeof(int));

        for (int i = 0; i < double_variable_count; i++)
        {
            strongly_connected_components[i] = i;
        }

        for(int i = 0; i < double_variable_count; i++)
        {
            for(int j = 0; j < double_variable_count; j++)
            {
                if (i != j)
                {
                    if (strongly_connected_components[i] != strongly_connected_components[j])
                    {
                        if ((implication_graph[i * double_variable_count + j] == 1) &&
                            (implication_graph[j * double_variable_count + i] == 1))
                        {
                            if (strongly_connected_components[j] > strongly_connected_components[i])
                            {
                                strongly_connected_components[j] = strongly_connected_components[i];
                            }
                            else
                            {
                                strongly_connected_components[i] = strongly_connected_components[j];
                            }
                        }
                    }
                }
            }
        }

        int* components = (int*)malloc(double_variable_count * sizeof(int));
        for (int i = 0; i < double_variable_count; i++)
        {
            components[i] = -1;
        }

        for (int i = 0; i < double_variable_count; i++)
        {
            if (components[strongly_connected_components[i]] == -1)
            {
                components[strongly_connected_components[i]] = i;
            }
        }

        int component_count = 0;
        int* component_map = (int*)malloc(double_variable_count*sizeof(int));
        for (int i = 0; i < double_variable_count; i++)
        {
            component_map[i] = -1;
        } 

        for (int i = 0; i < double_variable_count; i++)
        {
            if (components[i] != -1)
            {
                component_map[i] = component_count;
                component_count++;
            }
        }

        free(components);

        for (int i = 0; i < double_variable_count; i++)
        {
            strongly_connected_components[i] = component_map[strongly_connected_components[i]];
        }

        free(component_map);

        int* component_vertex = (int*)malloc(component_count * sizeof(int));
        for (int i = 0; i < component_count; i++)
        {
            component_vertex[i] = -1;
        }

        for (int i = 0; i < double_variable_count; i++)
        {
            if (component_vertex[strongly_connected_components[i]] == -1)
            {
                component_vertex[strongly_connected_components[i]] = i;
            }
        }

        int* component_topological_order = (int*)malloc(component_count * sizeof(int));

        for (int i = 0; i < component_count; i++)
        {
            int component_found = 0;
            int component_number = component_count - 1;
            for (int j = 0; (j < component_count) && (component_found == 0); j++)
            {
                if (component_vertex[j] != -1)
                {
                    int path_found = 0;
                    for (int k = 0; (k < component_count) && (path_found == 0); k++)
                    {
                        if (component_vertex[k] != -1)
                        {
                            if (k != j)
                            {
                                if (implication_graph[
                                    component_vertex[k] * double_variable_count +
                                    component_vertex[j]] == 1)
                                {
                                    path_found = 1;
                                }
                            }
                        }
                    }
                    if (path_found == 0)
                    {
                        component_found = 1;
                        component_vertex[j] = -1;
                        component_number = j;
                    }
                }
            }
            
            component_topological_order[component_number] = i;
        }

        free(component_vertex);            

        for (int i = 0; i < double_variable_count; i++)
        {
            strongly_connected_components[i] = component_topological_order[strongly_connected_components[i]];
        }

        free(component_topological_order);

        twosat_solution(variable_count, strongly_connected_components, model);

        free(strongly_connected_components);
    }

    return result;
}