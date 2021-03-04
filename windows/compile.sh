mkdir build
rm build/*

version=$(git describe --tags --abbrev=0)
## version contain the version of the last tag on github
version=1.3.15
qmake writernote.pro DEFINES+=VERSION_SNAPCRAFT=$version
make release -j $nproc 

ldd ./build/writernote.exe | grep '\/mingw.*\.dll' -o | sort -u | xargs -I{} cp "{}" ./build

windeployqt.exe ./build/writernote.exe --force