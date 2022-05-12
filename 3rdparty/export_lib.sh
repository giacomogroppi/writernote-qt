echo "Export lib for writernote. pwd:" $(pwd)
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/writernote-qt/3rdparty/poppler/install
export LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH

