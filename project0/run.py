import os

size = 16384
tries = 1
for threads in [1, 4]:#2, 4, 6, 8]:
  print "NUMT = %d" % threads
  print "SIZE = %d" % size
  print "NUMTRIES = %d" % tries
  cmd = "g++ -DNUMT=%d -DSIZE=%d -DNUMTRIES=%d project0.c -o project0 -lm -fopenmp" % (threads, size, tries)
  os.system(cmd)
  cmd = "./project0"
  os.system(cmd)