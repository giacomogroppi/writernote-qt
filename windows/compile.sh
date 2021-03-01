mkdir build
rm build/*

qmake writernote.pro 
make release -j $nproc 

ldd ./build/writernote.exe | grep '\/mingw.*\.dll' -o | sort -u | xargs -I{} cp "{}" ./build

windeployqt.exe ./build/writernote.exe --force
