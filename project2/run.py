import os

for threads in [1, 2, 4, 8, 16]:
  for nodes in [8, 16, 32, 64, 128, 256, 512, 1024, 2048, 8192]:
    cmd = "g++ -DNUMT=%d -DNUMNODES=%d project2.c -o project2 -lm -fopenmp" % (threads, nodes)
    os.system(cmd)
    cmd = "./project2"
    os.system(cmd)