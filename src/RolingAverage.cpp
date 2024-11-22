double array_calculate_avg(int * buf, int len)
{
    int sum = 0;

    for (int i = 0; i < len; ++i)
    {
        sum += buf[i];
    }

    return ((double) sum) / ((double) len);
}