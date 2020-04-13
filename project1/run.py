import os

tries = 100
for threads in [1, 2, 4, 6, 8, 16]:
  for trials in [1000, 10000, 100000, 1000000]:
    cmd = "g++ -DNUMT=%d -DNUMTRIALS=%d -DNUMTRIES=%d project1.c -o project1 -lm -fopenmp" % (threads, trials, tries)
    os.system(cmd)
    cmd = "./project1"
    os.system(cmd)