old=$(pwd)
cd ..

echo "PATH: " $(pwd)

cd 3rdparty
PREFIX_LIB=$(pwd)

cd zlib-ng

rm -r build
rm -r install

mkdir build
mkdir install
cd build

cmake .. \
    -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/zlib-ng/install \
    -DBIN_INSTALL_DIR=$PREFIX_LIB/zlib-ng/install/bin \
    -DLIB_INSTALL_DIR=$PREFIX_LIB/zlib-ng/install/lib \
    -DINC_INSTALL_DIR=$PREFIX_LIB/zlib-ng/install/include \
    -DPKGCONFIG_INSTALL_DIR=$PREFIX_LIB/zlib-ng/install/lib/pkgconfig \
    -DMAN_INSTALL_DIR=$PREFIX_LIB/zlib-ng/install/share/man \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DZLIB_COMPAT=ON \
    -DZLIB_ENABLE_TESTS=OFF

cmake --build . --config Release --target install

echo "Prefix lib: " $PREFIX_LIB

cd $PREFIX_LIB/libzip

rm -r build
rm -r install

mkdir build
mkdir install

cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/libzip/install \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_BZIP2=OFF \
    -DENABLE_LZMA=OFF \
    -DENABLE_COMMONCRYPTO=OFF \
    -DENABLE_GNUTLS=OFF \
    -DENABLE_MBEDTLS=OFF \
    -DENABLE_OPENSSL=OFF \
    -DENABLE_WINDOWS_CRYPTO=OFF \
    -DBUILD_DOC=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_REGRESS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_TOOLS=OFF \
    -DZLIB_INCLUDE_DIR=$PREFIX_LIB/zlib-ng/install/include \
    -DZLIB_LIBRARY_DEBUG=$PREFIX_LIB/zlib-ng/install/lib/libzlib.a \
    -DZLIB_LIBRARY_RELEASE=$PREFIX_LIB/zlib-ng/install/lib/libzlib.a

cmake --build . --config Release --target install