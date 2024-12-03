# NVRTC mem leak reproduction

ref:  https://forums.developer.nvidia.com/t/memory-leak-when-using-nvjitlinkadddata-nvjitlinkaddfile-in-cuda-jit-compilation/315401/1

## Cuda version
cuda 12.4

## Build
```bash
make build
```

## Run
```bash
./a.out
```

## Observe the `top` command results
```bash
top -d1 -p $(pgrep -d, a.out)
```

The value of the `RES` column in the `top` command keeps increasing
