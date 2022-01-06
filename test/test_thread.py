import sys
import time
import os

make = "make"
qmake = "qmake"
force_thread = "DEBUG_THREAD"
writernote = "./build/writernote"

min = 8
max = 20

def start(command: str) -> int:
    __time = time.clock_gettime_ns()

    os.system(command)

    return time.clock_gettime_ns() - __time

def build(thread: int) -> bool:
    os.system("{} clean".format(make))
    command = "{} DEFINES+={}={} ; {} -j$(nproc)".format(qmake, force_thread, thread, make)

    return os.system(command)


def start_test(name_file: str) -> int:
    command = "{} {}".format(writernote, name_file)

    return start(command)

def for_each_file() -> list[int]:
    res = []
    for x in range(1, 6):
        name_file = "file{}.writer".format(str(x))

        res.append(start_test(name_file))

    return res

def main() -> None:
    thread = [8, 10, 14, 18, 22]
    res = []
    for threadCount in thread:
        if build(threadCount):
            print("Build error")
            exit()

        res.append(for_each_file())

    for threadSingle, x in zip(thread, res):
        for time in x:
            print("threadSingle: {} time: {}".format(x, time))

if __name__ == "__main__":
    if sys.platform != "linux":
        print("This script only work on linux")
        exit()

    main()