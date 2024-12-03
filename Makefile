
constant.o: constant.cu
	nvcc -t0 -gencode=arch=compute_80,code=sm_80 -rdc=true -Xcompiler -fPIC -c constant.cu

non-constant.o: non-constant.cu
	nvcc -t0 -gencode=arch=compute_80,code=sm_80 -rdc=true -Xcompiler -fPIC -c non-constant.cu


build: constant.o non-constant.o
	nvcc -std=c++14 ./main.cc -lnvrtc -lnvJitLink -ldl
