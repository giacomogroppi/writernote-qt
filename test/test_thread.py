import sys
import time
import os

make = "make"
qmake = "qmake"
force_thread = "DEBUG_THREAD"
writernote = "./build/writernote"
build_path = "build"

min = 8
max = 20

def start(command: str) -> None:
    os.system(command)


def build(thread: int) -> bool:
    os.system("{} clean".format(make))
    command = "{} DEFINES+={}={} ; {} -j$(nproc)".format(qmake, force_thread, thread, make)

    return os.system(command)


def start_test(name_file: str) -> int:
    command = "{} {}".format(writernote, name_file)

    return start(command)

def decode_test() -> int:
    pos_file = "{}/time.txt".format(build_path)
    
    res = []
    
    try:
        with open(pos_file, "r") as file:
            res.append(int(file.readline()))

    except FileNotFoundError:
        print("File {} not found".format(pos_file))

    return res

def for_each_file() -> int:
    name_file = "file.writer"
    
    start_test(name_file)

    res = decode_test()

    return res

def main() -> None:
    thread = [8, 10, 14, 18, 22]
    res = []
    for threadCount in thread:
        if build(threadCount):
            print("Build error")
            exit()

        res.append(for_each_file())

    for threadSingle, time in zip(thread, res):
        print("threadSingle: {} time: {}".format(threadSingle, time))

if __name__ == "__main__":
    os.system("{} clean".format(make))

    if sys.platform != "linux":
        print("This script only work on linux")
        exit()

    main()