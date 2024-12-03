
__constant__ long long some_constant[1024] = {1, 2, 3, 4};

__device__ void foo(long long *arg)
{
    for (int i = 0; i < 1024; i++)
    {
        arg[i] = some_constant[i];
    }
}
