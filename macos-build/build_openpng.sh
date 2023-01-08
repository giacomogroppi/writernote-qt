
git clone -b v1.6.37 https://github.com/glennrp/libpng.git libpng

cd libpng

rm INSTALL

mkdir build
mkdir install

cd build
cmake .. \
    -DPNG_STATIC=ON \
    -DPNG_SHARED=OFF \
    -DCMAKE_PREFIX_PATH=$PREFIX_LIB \
    -DZLIB_INCLUDE_DIR=$PREFIX_LIB/zlib-ng/distrib/include \
    -DZLIB_LIBRARY_DEBUG=$PREFIX_LIB/zlib-ng/distrib/lib/libz.a \
    -DZLIB_LIBRARY_RELEASE=$PREFIX_LIB/zlib-ng/distrib/lib/libz.a \
    -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/libpng/install

make -j$(nproc)
sudo make install