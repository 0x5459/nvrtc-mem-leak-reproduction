
__device__ void foo(long long *arg)
{
    for (int i = 0; i < 1024; i++)
    {
        arg[i] = arg[i + 1] + arg[i + 2];
    }
}
