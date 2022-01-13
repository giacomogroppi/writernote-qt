#!/usr/bin/python3
import sys
import os

OPTIONS = [
    "compile",
    "clean",
    "create",
    "log"
]

def print_command() -> None:
    print("multi.py ", OPTIONS)
    exit(-1)

try:
    command = sys.argv[1]
    if not command in OPTIONS:
        print_command()
except IndexError:
    print_command()

CPU = 8
RAM = 10
DISK = 100
LOG_POSITION = "log_snapcraft.txt"

if command == 'compile':
    exec = """
    snapcraft clean
    multipass launch --name snapcraft-writernote --cpus {} --mem {}G --disk {}G
    snapcraft""".format(CPU, RAM, DISK)

elif command == 'clean':
    exec = "snapcraft clean"

elif command == 'create':
    exec = "multipass launch --name snapcraft-writernote --cpus {} --mem {}G --disk {}G".format(CPU, RAM, DISK)

elif command == 'log':
    exec = "snapcraft > {}".format(LOG_POSITION)

os.system(exec)