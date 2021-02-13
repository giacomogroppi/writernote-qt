it contains libzip for android

to download the right library of libzip run lib_libzip_compile.sh
to compile it run ./build.sh

to compile android application you need a openjdk 8, and gradle
after that run qmake writernote.pro on qtcreator, it they will automatically generate the make file
with the right compiler for android, and than run make -j($nproc).
after that you should have a .so image in your android-build.

To create the .aab or apk run

make aab

or 

make apk

