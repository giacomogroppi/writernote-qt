export PATH="$HOME/.new_local/bin:$PATH"
cd $PREFIX_LIB

cd poppler || exit
          
mkdir build
          
cd build || exit
pwd
          
ls $QT_PREFIX/lib/cmake

cmake .. \
  -DCMAKE_PREFIX_PATH=$QT_PREFIX/lib/cmake \
  -DENABLE_BOOST=OFF \
  -DENABLE_QT5=OFF \
  -DENABLE_ZLIB=OFF \
  -DENABLE_LIBCURL=OFF \
  -DENABLE_GLIB=OFF \
  -DENABLE_CPP=OFF \
  -DENABLE_QT6=ON \
  -DJPEG_DIR=$QT_PREFIX/openjpeg_build/lib/openjpeg-2.3/ \
  -DZLIB_INCLUDE_DIR=$PREFIX_LIB/zlib-ng/distrib/include \
  -DZLIB_LIBRARY_RELEASE=$PREFIX_LIB/zlib-ng/distrib/lib/libz.a \
  -DPNG_INCLUDE_DIR=$PREFIX_LIB/libpng/install/include/ \
  -DPNG_LIBRARY_RELEASE=$PREFIX_LIB/libpng/install/lib/libpng.a \
  -DJPEG_LIBRARY_RELEASE=$PREFIX_LIB/openjpeg-2.3.0/build/bin/libopenjp2.a \
  -DJPEG_INCLUDE_DIR=$PREFIX_LIB/openjpeg_build/include/openjpeg-2.3 \
  -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/poppler \
  -DFreetype_INCLUDE_DIR=$PREFIX_LIB/freetype/install/include/freetype2/freetype/ \
  -DFreetype_LIBRARY_RELEASE=$PREFIX_LIB/freetype/install/lib/libfreetype.a
  -DBUILD_SHARED_LIBS=OFF 

make -j$(nproc)
sudo make install