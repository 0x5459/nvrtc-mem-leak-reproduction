# NVRTC mem leak reproduction

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
top -d1 -p $(pgrep -d, a.out)ted
```
The value of the `RES` column in the `top` command keeps increasing