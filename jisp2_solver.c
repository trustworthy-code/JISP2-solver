#include <stdlib.h>
#include "interval_intersection_check.h"
#include "two_satisfiability_solver.h"

int jisp2_solver(int job_count, int* job_interval_endpoints, int* schedule)
{
    int result = 0;

    int disjunction_count = 0;
    int* twocnf_formula = (int*)malloc((4*job_count + 3* job_count * (job_count - 1)) * sizeof(int));
    int* variable_interval_map = (int*)malloc(2 * job_count * sizeof(int));
    int* job_variable_map = (int*)malloc(job_count * sizeof(int));
    int variable_count = 0;

    for(int i = 0; i < job_count; i++)
    {
        int index = i * 4;
        if ((job_interval_endpoints[index + 2] == 0) &&
            (job_interval_endpoints[index + 3] == 0))
        {
            variable_count++;
            int variable = variable_count;
            variable_interval_map[variable-1] = index;
            job_variable_map[i] = variable;
            twocnf_formula[2 * disjunction_count] = variable;
            twocnf_formula[2 * disjunction_count + 1] = variable;
            disjunction_count++;
        }
        else
        {
            variable_count++;
            int first_variable = variable_count;
            variable_count++;
            int second_variable = variable_count;
            variable_interval_map[first_variable-1] = index;
            job_variable_map[i] = first_variable;
            variable_interval_map[second_variable-1] = index + 2;
            twocnf_formula[2 * disjunction_count] = first_variable;
            twocnf_formula[2 * disjunction_count + 1] = second_variable;
            disjunction_count++;
            twocnf_formula[2 * disjunction_count] = -first_variable;
            twocnf_formula[2 * disjunction_count + 1] = -second_variable;
            disjunction_count++;
        }
    }

    int infeasible = 0;

    for(int i = 0; (i < job_count) && (infeasible == 0); i++)
    {
        int index_i = i * 4;
        for(int j = 0; (j < i) && (infeasible == 0); j++)
        {
            int index_j = j * 4;
            if ((job_interval_endpoints[index_i + 2] == 0) &&
                (job_interval_endpoints[index_i + 3] == 0) &&
                (job_interval_endpoints[index_j + 2] == 0) &&
                (job_interval_endpoints[index_j + 3] == 0))
            {
                if (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1)
                {
                    infeasible = 1;
                }
            }
            else if ((job_interval_endpoints[index_i + 2] == 0) &&
                     (job_interval_endpoints[index_i + 3] == 0))
            {
                if ((interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1) &&
                    (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j + 2],
                                                job_interval_endpoints[index_j + 3])
                                                == 1))
                {
                    infeasible = 1;
                }
                else if (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1)
                {
                    twocnf_formula[2 * disjunction_count] = -job_variable_map[i];
                    twocnf_formula[2 * disjunction_count + 1] = -job_variable_map[j];
                    disjunction_count++;
                }
                else if (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j + 2],
                                                job_interval_endpoints[index_j + 3])
                                                == 1)
                {
                    twocnf_formula[2 * disjunction_count] = -job_variable_map[i];
                    twocnf_formula[2 * disjunction_count + 1] = -(job_variable_map[j] + 1);
                    disjunction_count++;
                }
            }
            else if ((job_interval_endpoints[index_j + 2] == 0) &&
                     (job_interval_endpoints[index_j + 3] == 0))
            {
                if ((interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1) &&
                    (interval_intersection_check(job_interval_endpoints[index_i + 2],
                                                job_interval_endpoints[index_i + 3],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1))
                {
                    infeasible = 1;
                }
                else if (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1)
                {
                    twocnf_formula[2 * disjunction_count] = -job_variable_map[i];
                    twocnf_formula[2 * disjunction_count + 1] = -job_variable_map[j];
                    disjunction_count++;
                }
                else if (interval_intersection_check(job_interval_endpoints[index_i + 2],
                                                job_interval_endpoints[index_i + 3],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1)
                {
                    twocnf_formula[2 * disjunction_count] = -(job_variable_map[i] + 1);
                    twocnf_formula[2 * disjunction_count + 1] = -job_variable_map[j];
                    disjunction_count++;
                }
            }
            else
            {
                if ((interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1) &&
                    (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j + 2],
                                                job_interval_endpoints[index_j + 3])
                                                == 1) &&
                    (interval_intersection_check(job_interval_endpoints[index_i + 2],
                                                job_interval_endpoints[index_i + 3],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1) &&
                    (interval_intersection_check(job_interval_endpoints[index_i + 2],
                                                job_interval_endpoints[index_i + 3],
                                                job_interval_endpoints[index_j + 2],
                                                job_interval_endpoints[index_j + 3])
                                                == 1))
                {
                    infeasible = 1;
                }
                else
                {
                    if (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1)
                    {
                        twocnf_formula[2 * disjunction_count] = -job_variable_map[i];
                        twocnf_formula[2 * disjunction_count + 1] = -job_variable_map[j];
                        disjunction_count++;
                    }

                    if (interval_intersection_check(job_interval_endpoints[index_i],
                                                job_interval_endpoints[index_i + 1],
                                                job_interval_endpoints[index_j + 2],
                                                job_interval_endpoints[index_j + 3])
                                                == 1)
                    {
                        twocnf_formula[2 * disjunction_count] = -job_variable_map[i];
                        twocnf_formula[2 * disjunction_count + 1] = -(job_variable_map[j] + 1);
                        disjunction_count++;
                    }

                    if (interval_intersection_check(job_interval_endpoints[index_i + 2],
                                                job_interval_endpoints[index_i + 3],
                                                job_interval_endpoints[index_j],
                                                job_interval_endpoints[index_j + 1])
                                                == 1)
                    {
                        twocnf_formula[2 * disjunction_count] = -(job_variable_map[i] + 1);
                        twocnf_formula[2 * disjunction_count + 1] = -job_variable_map[j];
                        disjunction_count++;
                    }

                    if (interval_intersection_check(job_interval_endpoints[index_i + 2],
                                                job_interval_endpoints[index_i + 3],
                                                job_interval_endpoints[index_j + 2],
                                                job_interval_endpoints[index_j + 3])
                                                == 1)
                    {
                        twocnf_formula[2 * disjunction_count] = -(job_variable_map[i] + 1);
                        twocnf_formula[2 * disjunction_count + 1] = -(job_variable_map[j] + 1);
                        disjunction_count++;
                    }
                }
            }           
        }
    }

    free(job_variable_map);

    if (infeasible == 1)
    {
        result = 0;
    }
    else
    {
        int* formula = (int*)malloc(2 * disjunction_count * sizeof(int));

        for (int i = 0; i < disjunction_count; i++)
        {
            formula[2 * i] = twocnf_formula[2 * i];
            formula[2 * i + 1] = twocnf_formula[2 * i + 1];
        }

        free(twocnf_formula);    

        int* model = (int*)malloc(variable_count * sizeof(int));
        int satisfiable = two_satisfiability_solver(variable_count, disjunction_count, formula, model);

        if (satisfiable == 0)
        {
            result = 0;
        }
        else
        {
            result = 1;
       
            for(int i = 0; i < variable_count; i++)
            {
                if (model[i] > 0)
                {
                    int job = variable_interval_map[i] / 4;
                    schedule[2 * job] = job_interval_endpoints[variable_interval_map[i]];
                    schedule[2 * job + 1] = job_interval_endpoints[variable_interval_map[i] + 1];
                }
            }
        }

        free(formula);
        free(variable_interval_map);
        free(model);
    }

    return result;
}