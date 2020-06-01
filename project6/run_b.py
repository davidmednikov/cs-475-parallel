import os

for num_elements in [1024, 2048, 8192, 32768, 131072, 262144, 1048576, 4194304, 8388608]:
    for local_size in [8, 16, 32, 64, 128, 256, 512]:
        cmd = "g++ -DNUM_ELEMENTS=%d -DLOCAL_SIZE=%d -o array_mult_add array_mult_add.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp"% (num_elements, local_size)
        os.system(cmd)
        cmd = "./array_mult_add"
        os.system(cmd)