mkdir build
rm build/*

qmake writernote.pro
make

windeployqt.exe ./build/writernote.exe --force

ldd ./build/writernote.exe | grep '\/mingw.*\.dll' -o | sort -u | xargs -I{} cp "{}" ./build
