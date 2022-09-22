import os
import sys

if len(sys.argv) != 2:
    print("For export call export_ver_qt.py POS_QT")
    exit(-1)

c = "export QT_VER_WRITERNOTE={}".format(sys.argv[1])
os.system(c)