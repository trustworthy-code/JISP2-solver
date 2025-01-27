void twosat_solution(int variable_count, int* strongly_connected_components, int* model)
{
    int x = 0;

    while (x < variable_count)
    {
        if (strongly_connected_components[x] > strongly_connected_components[x + variable_count])
        {
            model[x] = x + 1;
        }
        else
        {
            model[x] = -(x + 1);
        }
        x++;
    }
}