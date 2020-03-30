import os

for i in [1, 2, 4, 6, 8]:
  print "NUMT = %d" % t
  for s in [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 3072, 4096]:
    print "NUMS = %d" % s
    cmd = "g++ -DNUMS=%d -DNUMT=%d project.c -o project -lm -fopenmp" % (s, t)
    os.system(cmd)
    cmd = "./project"
    os.system(cmd)