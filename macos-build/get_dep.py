import sys
import os
from typing import List

COMMAND = "otool -L "
SUFF_LIB = "Contents/Resources/lib/"
SUFF = "Contents/MacOS/writernote"
COPY = "cp "

COMMAND_TOOL = "install_name_tool "
COMMAND_SUFF = " -change "
COMMAND_BEFORE = " @rpath"

def move_single(path_lib: str) -> bool:
    return os.system(COMMAND_TOOL + COMMAND_SUFF + COMMAND_BEFORE + path_lib)


def save_dep(pos_binary: str, dest_list:str) -> None:
    string = ""

    os.system(COMMAND + pos_binary + " >> " + dest_list)

def get_dep(pos_lib: str) -> list[str]:
    list = []

    with open(pos_lib, "r") as file:
        lines = file.readlines()
        for line in lines:
            list.append(line)

    return list


def analise(list: list[str]) -> list[str]:
    list_sec = []
    i = 0
    for line in list:
        list_sec.append(line)
        list_sec[i] = list_sec[i][1:]

        try:
            ind = list_sec[i].index('(')
            list_sec[i] = list_sec[i][:ind-1]
        except:
            pass

        list_sec[i].replace("\n", "")
        list_sec[i].replace("\t", "")

        try:
            ind = list_sec[i].index('/usr/local')
            #print("not delete: ", list_sec[i])
        except:
            #print("delete: ", list_sec[i])
            del list_sec[i]
            i -= 1


        i += 1

    return list_sec

def remove_double(list_dep: list[str]) -> list[str]:
    newlist = []
    for i in list_dep:
        if i not in newlist:
            newlist.append(i)

    return newlist

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
        if os.system(COPY + dep + " " + ref ) != 0 or move_single(ref):
            return False

    return True

if __name__ == "__main__":
    pos_bin = sys.argv[1]

    ind = pos_bin.index("writernote.app")
    pos_dest = pos_bin[:ind] + "lib_list.txt"

    save_dep(pos_bin + "/" + SUFF, pos_dest)

    list = get_dep(pos_dest)
    list = analise(list)
    list = remove_double(list)

    if not copy_dep(pos_bin, list):
        print("Error copy")
        exit(1)
    else:
        print("Ok")
        exit(0)