from os import system, name
from time import sleep
import filecmp


def compile_file(path):
    system("g++.exe " + path + ".cpp -o " + path + ".exe")


#enter full paths to generator, good solution, bad solution
path_gen = "???"
path_ok = "???"
path_bad = "???"


def go():
    system(path_gen+".exe > gen_output.txt")
    system(path_ok + ".exe < gen_output.txt > ok_output.txt")
    system(path_bad + ".exe < gen_output.txt > bad_output.txt")
    f1 = ".\\ok_output.txt"
    f2 = ".\\bad_output.txt"
    res = filecmp.cmp(f1, f2)
    if not res:
        print("Input:")
        print(open(".\\gen_output.txt").read())
        print()
        print("Good:")
        print(open(f1).read())
        print()
        print("Bad:")
        print(open(f2).read())
        print()
    return res


def clear():
    # for windows
    if name == 'nt':
        _ = system('cls')

    # for mac and linux(here, os.name is 'posix')
    else:
        _ = system('clear')

print("Compiling generator...")
compile_file(path_gen)
print("Compiling good solution...")
compile_file(path_ok)
print("Compiling bad solution...")
compile_file(path_bad)
found = False
for i in range(500):
    print("----------------- Test", i+1, "-----------------")
    res = go()
    if not res:
    	found = True
    	break
system("del .\\ok_output.txt")
system("del .\\gen_output.txt")
system("del .\\bad_output.txt")
if found:
	print("Found!")
else:
    print("Not found!")