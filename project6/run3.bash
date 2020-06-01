#!/bin/bash
#SBATCH -J Project6-ArrayMultReduce
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o project6.o
#SBATCH -e project6.e
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=mednikod@oregonstate.edu
for num_elements in 1024 2048 8192 32768 131072 262144 1048576 4194304 8388608
do
for local_size in 32 64 128 256
do
g++ -DNUM_ELEMENTS=$num_elements -DLOCAL_SIZE=$local_size -o array_mult_reduce array_mult_reduce.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
./array_mult_reduce
done
done