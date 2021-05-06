mkdir build
rm build/*

git pull

version=$(git describe --tags --abbrev=0)
## version contain the version of the last tag on github

/mingw64/bin/qmake writernote.pro DEFINES+="VERSION_SNAPCRAFT=$version" DEFINES+=QT_NO_DEBUG_OUTPUT
make release -j $nproc 

ldd ./build/writernote.exe | grep '\/mingw.*\.dll' -o | sort -u | xargs -I{} cp "{}" ./build

/mingw64/bin/windeployqt.exe ./build/writernote.exe --force

cd updater
/mingw64/bin/qmake updater.pro
/mingw64/bin/windeployqt.exe ./updater.exe --force

