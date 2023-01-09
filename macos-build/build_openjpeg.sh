cd $PREFIX_LIB

mkdir openjpeg_build

curl -L https://github.com/uclouvain/openjpeg/archive/v2.3.0.tar.gz -o openjpeg.tar.gz
tar xf openjpeg.tar.gz
mv openjpeg-* openjpeg
cd openjpeg || exit
mkdir build
cd build || exit

pwd
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/openjpeg_build \
    -DBUILD_STATIC_LIBS=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DPNG_INCLUDE_DIR=$PREFIX_LIB/libpng/install/include/ \
    -DPNG_LIBRARY_RELEASE=$PREFIX_LIB/libpng/install/lib/libpng.a \
    -DZLIB_INCLUDE_DIR=$PREFIX_LIB/zlib-ng/distrib/include \
    -DZLIB_LIBRARY_RELEASE=$PREFIX_LIB/zlib-ng/distrib/lib/libz.a \
    -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install