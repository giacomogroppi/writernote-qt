old=$(pwd)
cd ..
cd 3rdparty

PREFIX_LIB=$(pwd)

cd poppler
mkdir build
mkdir install

cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/poppler \
    -DENABLE_CPP=OFF \
    -DENABLE_GLIB=OFF \
    -DENABLE_QT5=OFF \
    -DENABLE_UTILS=OFF


ninja
ninja install