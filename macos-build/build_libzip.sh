
cd libzip

echo $(pwd)
mkdir build
cd build
cmake .. -B$PREFIX_LIB/libzip \
  -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/libzip/distrib \
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
  -DZLIB_INCLUDE_DIR=$PREFIX_LIB/zlib-ng/distrib/include \
  -DZLIB_LIBRARY_DEBUG=$PREFIX_LIB/zlib-ng/distrib/lib/libz.a \
  -DZLIB_LIBRARY_RELEASE=$PREFIX_LIB/zlib-ng/distrib/lib/libz.a

cmake --build $PREFIX_LIB/libzip/ --config Release --target install