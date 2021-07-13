qmake
make -j $(nproc)
python3 macos-build/get_deb.py $(pwd)build/writernote.app