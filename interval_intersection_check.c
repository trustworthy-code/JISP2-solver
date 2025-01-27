int interval_intersection_check(int first_interval_start, int first_interval_finish,
                                int second_interval_start, int second_interval_finish)
{
    int result = 0;
    if ((first_interval_start <= second_interval_start) &&
        (first_interval_finish > second_interval_start))
    {
        result = 1;
    } 
    else if ((second_interval_start <= first_interval_start) &&
        (second_interval_finish > first_interval_start))
    {
        result = 1;
    } 
    return result;
}