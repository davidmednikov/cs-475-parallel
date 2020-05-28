import os

for size in [1000, 10000, 50000, 100000, 500000, 1000000, 2000000, 4000000, 8000000]:
  for tries in [5, 10, 100, 1000]:
    my_prog_cmd = "g++ -DSIZE=%d -DNUMTRIES=%d project4.cpp -o project4 -lm -fopenmp -O3" % (size, tries)
    os.system(my_prog_cmd)
    my_prog_cmd = "./project4"
    os.system(my_prog_cmd)