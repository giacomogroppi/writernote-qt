import sys
import os

COMMAND = "otool -L "
SUFF = "Contents/MacOS/writernote"

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
    for i, line in enumerate(list):
        list_sec.append(line)
        list_sec[i] = list_sec[i][1:]

        try:
            ind = list_sec[i].index('(')
    
            list_sec[i] = list_sec[i][:ind-1]

        except:
            pass

    return list

if __name__ == "__main__":
    pos_bin = sys.argv[1]

    ind = pos_bin.index("writernote.app")
    pos_dest = pos_bin[:ind] + "lib_list.txt"

    save_dep(pos_bin + "/" + SUFF, pos_dest)

    print(get_dep(pos_dest))
