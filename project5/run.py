import os

for block_size in [16, 32, 64, 128]:
  for trials in [16384, 32768, 65536, 131072, 262144, 524288, 1048576]:
    cmd = "/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DNUMTRIALS=%d -DBLOCKSIZE=%d -o project5 project5.cu" % (trials, block_size)
    os.system(cmd)
    cmd = "./project5"
    os.system(cmd)