#include <nvrtc.h>
#include <cuda.h>
#include <nvJitLink.h>
#include <nvrtc.h>
#include <iostream>

#define NVRTC_SAFE_CALL(x)                                    \
    do                                                        \
    {                                                         \
        nvrtcResult result = x;                               \
        if (result != NVRTC_SUCCESS)                          \
        {                                                     \
            std::cerr << "\nerror: " #x " failed with error " \
                      << nvrtcGetErrorString(result) << '\n'; \
            exit(1);                                          \
        }                                                     \
    } while (0)
#define CUDA_SAFE_CALL(x)                                     \
    do                                                        \
    {                                                         \
        CUresult result = x;                                  \
        if (result != CUDA_SUCCESS)                           \
        {                                                     \
            const char *msg;                                  \
            cuGetErrorName(result, &msg);                     \
            std::cerr << "\nerror: " #x " failed with error " \
                      << msg << '\n';                         \
            exit(1);                                          \
        }                                                     \
    } while (0)
#define NVJITLINK_SAFE_CALL(h, x)                             \
    do                                                        \
    {                                                         \
        nvJitLinkResult result = x;                           \
        if (result != NVJITLINK_SUCCESS)                      \
        {                                                     \
            std::cerr << "\nerror: " #x " failed with error " \
                      << result << '\n';                      \
            size_t lsize;                                     \
            result = nvJitLinkGetErrorLogSize(h, &lsize);     \
            if (result == NVJITLINK_SUCCESS && lsize > 0)     \
            {                                                 \
                char *log = (char *)malloc(lsize);            \
                result = nvJitLinkGetErrorLog(h, log);        \
                if (result == NVJITLINK_SUCCESS)              \
                {                                             \
                    std::cerr << "error: " << log << '\n';    \
                    free(log);                                \
                }                                             \
            }                                                 \
            exit(1);                                          \
        }                                                     \
    } while (0)

int main()
{

    std::string program_name = "my_program";
    std::string program_source = R"(
  template <typename T>
  __global__ void my_kernel(T* data) { *data = T{7}; }
  )";

    for (int i = 0; i < 80000; i++)
    {
        nvrtcProgram prog;
        NVRTC_SAFE_CALL(
            nvrtcCreateProgram(&prog,                  // prog
                               program_source.c_str(), // buffer
                               program_name.c_str(),   // name
                               0,                      // numHeaders
                               NULL,                   // headers
                               NULL));                 // includeNames

        nvrtcResult compileResult = nvrtcCompileProgram(prog,  // prog
                                                        0,     // numOptions
                                                        NULL); // options
                                                               // Obtain compilation log from the program.

        
        if (compileResult != NVRTC_SUCCESS)
        {
            exit(1);
        }

        nvJitLinkHandle handle;
        const char *lopts[] = {"-arch=sm_80"};
        NVJITLINK_SAFE_CALL(handle, nvJitLinkCreate(&handle, 1, lopts));
        NVJITLINK_SAFE_CALL(handle, nvJitLinkAddFile(handle, NVJITLINK_INPUT_OBJECT,
                                                    //  "./constant.o"));
                                                     "./non-constant.o"));
        NVJITLINK_SAFE_CALL(handle, nvJitLinkComplete(handle));
        NVJITLINK_SAFE_CALL(handle, nvJitLinkDestroy(&handle));
        NVRTC_SAFE_CALL(nvrtcDestroyProgram(&prog));
    }
}
