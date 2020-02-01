f_p = open("out.txt", "r")
f_c = open("out_c.txt", "r")

l_p = f_p.readline()
l_c = f_c.readline()
flag = True
while l_p:
    if(l_p != l_c):
        flag = False
        print("ERROR")
        print(l_p)
        print(l_c)
    l_p = f_p.readline()
    l_c = f_c.readline()

print(flag)