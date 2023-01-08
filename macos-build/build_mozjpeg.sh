cd $PREFIX_LIB
cd mozjpeg

git clone -b v4.0.1-rc2 https://github.com/mozilla/mozjpeg.git

cmake -B build . \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/mozjpeg \
  -DWITH_JPEG8=ON \
  -DPNG_SUPPORTED=OFF

cmake --build build -j$(nproc)
sudo cmake --install build