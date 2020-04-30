import os

cmd = "g++ project3.c -o project3 -lm -fopenmp"
os.system(cmd)
cmd = "./project3"
os.system(cmd)