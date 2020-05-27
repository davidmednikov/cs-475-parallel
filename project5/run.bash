#!/bin/bash
#SBATCH -J Project5
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o project5.out1
#SBATCH -e project5.err1
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=mednikod@oregonstate.edu
for block_size in 16 32 64 128
do
for trials in 16384 32768 65536 131072 262144 524288 1048576
do
/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DNUMTRIALS=$trials -DBLOCKSIZE=$block_size -o project5 project5.cu
./project5
done
done