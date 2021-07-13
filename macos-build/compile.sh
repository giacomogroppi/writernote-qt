qmake
make -j $(nproc)
python3 macos-build/get_dep.py $(pwd)build/writernote.app