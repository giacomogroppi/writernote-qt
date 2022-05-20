#!/usr/bin/python3
import sys
import os
import multiprocessing
import psutil

OPTIONS = [
    "compile",
    "clean",
    "create",
    "log"
]

def clear() -> None:
    exec = "snapcraft clean"
    os.system(exec)

def create(num_thread: int, num_ram: int, num_disk: int) -> None:
    exec = "multipass launch --name snapcraft-writernote --cpus {} --mem {}G --disk {}G".format(num_thread, num_ram, num_disk)
    os.system(exec)

def print_command() -> None:
    print("multi.py ", OPTIONS)
    exit(-1)

try:
    command = sys.argv[1]
    if not command in OPTIONS:
        print_command()
except IndexError:
    print_command()

CPU = 12
RAM = 10
DISK = 100
LOG_POSITION = "log_snapcraft.txt"

MAX_CPU = multiprocessing.cpu_count() * 2 / 3
MAX_RAM = (psutil.virtual_memory().total / (10**9)) / 2

CPU_USE = int(min(CPU, MAX_CPU))
MEM_USE = int(min(RAM, MAX_RAM))

print("Cpu use: {} Mem use: {}".format(CPU_USE, MEM_USE))

if command == 'compile':
    exec = """
    snapcraft clean
    multipass launch --name snapcraft-writernote --cpus {} --mem {}G --disk {}G
    snapcraft""".format(CPU_USE, MEM_USE, DISK)

elif command == 'clean':
    exec = "snapcraft clean"

elif command == 'create':
    exec = "multipass launch --name snapcraft-writernote --cpus {} --mem {}G --disk {}G".format(CPU_USE, MEM_USE, DISK)

elif command == 'log':
    exec = "snapcraft > {}".format(LOG_POSITION)

elif command == 'auto':
    clear()
    create(MAX_CPU, MAX_RAM, 50)

os.system(exec)
