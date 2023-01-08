cd $PREFIX_LIB/zlib-ng/

cmake . -B$PREFIX_LIB/zlib-ng/ \
  -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/zlib-ng/distrib \
  -DBIN_INSTALL_DIR=$PREFIX_LIB/zlib-ng/distrib/bin \
  -DLIB_INSTALL_DIR=$PREFIX_LIB/zlib-ng/distrib/lib \
  -DINC_INSTALL_DIR=$PREFIX_LIB/zlib-ng/distrib/include \
  -DPKGCONFIG_INSTALL_DIR=$PREFIX_LIB/zlib-ng/distrib/lib/pkgconfig \
  -DMAN_INSTALL_DIR=$PREFIX_LIB/zlib-ng/distrib/share/man \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DZLIB_COMPAT=ON \
  -DZLIB_ENABLE_TESTS=OFF

cmake --build $PREFIX_LIB/zlib-ng/ --config Release --target install