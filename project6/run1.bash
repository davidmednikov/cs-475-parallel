#!/bin/bash
#SBATCH -J Project6-ArrayMult
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o project6ArrayMult.out1
#SBATCH -e project6ArrayMult.err1
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=mednikod@oregonstate.edu
for num_elements in 1024 2048 8192 32768 131072 262144 1048576 4194304 8388608
do
for local_size in 8 16 32 64 128 256 512
do
g++ -DNUM_ELEMENTS=$num_elements -DLOCAL_SIZE=$local_size -o array_mult array_mult.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
./array_mult
done
done