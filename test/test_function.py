import os
import sys

def execute_writernote(pos_file : str, mode : str) -> int:
    return os.system("{} {}".format(pos_file, mode))

command = [
    "rubber",
    "square",
    "test",
    "evidenziatore",
]

try:
    pos_executable = sys.argv[1]
except: 
    print("You need to specify the position of the file")
    exit(-1)

for mode in command:
    print("{} {}".format(mode, execute_writernote()))