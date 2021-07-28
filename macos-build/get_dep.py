import sys
import os
from typing import List

COMMAND = "otool -L "
SUFF_LIB = "Contents/Resources/lib/"
SUFF = "Contents/MacOS/writernote"
COPY = "cp "

COMMAND_TOOL = "install_name_tool "
COMMAND_SUFF = " -change "
COMMAND_BEFORE = " @executable_path"

def move_single(path_lib: str) -> bool:
    return os.system(COMMAND_TOOL + COMMAND_SUFF + COMMAND_BEFORE + path_lib)


def remove_double(list_dep: list[str]) -> list[str]:
    newlist = []
    for i in list_dep:
        if i not in newlist:
            newlist.append(i)

    return newlist

def get_dep(pos_binary: str, dest_list: str) -> list[str]:
    os.system(COMMAND + pos_binary + " >> " + dest_list)
    list = []

    with open(dest_list, "r") as file:
        lines = file.readlines()
        for line in lines:
            list.append(line)

    list_sec = []
    i = 0
    for line in list:
        list_sec.append(line)

        try:
            ind = list_sec[i].index('(')
            list_sec[i] = list_sec[i][:ind]
        except ValueError:
            pass

        list_sec[i].replace(" ", "")
        list_sec[i].replace("\n", "")
        list_sec[i].replace("\t", "")

        try:
            ind = list_sec[i].index('/usr/local/opt')
        except:
            del list_sec[i]
            i -= 1

        i += 1

    return remove_double(list_sec)

def get_name_lib(lib: str) -> str:
    list = lib.split('/')
    return list[-1]

def copy_dep(app_path: str, list_dep: list[str]) -> bool:
    try:
        os.mkdir(app_path + "/" + SUFF_LIB)
    except:
        print("Directory already exist")

    for dep in list_dep:
        name = get_name_lib(dep)
        ref = app_path + "/" + SUFF_LIB + name
        if os.system(COPY + dep + " " + ref ) != 0: # or move_single(ref):
            return False

    return True

def print_dep(list: list[str]) -> None:
    for line in list:
        print(list)

pos_bin = sys.argv[1]
ind = pos_bin.index("writernote.app")
pos_dest = pos_bin[:ind] + "lib_list.txt"


def main(list: list[str]) -> list[str]:
    print("call to main")
    for dep in list:
        print("dep: ", dep)
        list_new_dep = get_dep(dep, pos_dest)

        list_new_dep = main(list_new_dep)
        print("list_new_dep ", list_new_dep)
        for dep_sec in list_new_dep:
            if dep_sec not in list:
                print("add ", dep_sec)
                list.append(dep_sec)

    return remove_double(list)

if __name__ == "__main__":
    
    pos_exe = pos_bin + "/" + SUFF

    list = get_dep(pos_exe, pos_dest)

    lista = main(list)

    print("List for all dep: ")
    print_dep(lista)

    if not copy_dep(pos_bin, list):
        print("Error copy")
        exit(1)
    else:
        print("Ok")
        exit(0)
