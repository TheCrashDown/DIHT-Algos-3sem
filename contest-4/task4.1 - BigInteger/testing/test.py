import random
import subprocess

fin = open("in.txt", "w")
fout = open("out.txt", "w")
for i in range(1000):
    a = random.randint(1, 2**50)
    b = random.randint(1, 2**20)

    res = a // b

    fin.write(str(a))
    fin.write("\n")
    fin.write(str(b))
    fin.write("\n")
    fout.write(str(res))
    fout.write("\n")
