cd $PREFIX_LIB

sudo rm -r mozjpeg

git clone -b v4.0.1-rc2 https://github.com/mozilla/mozjpeg.git

cd mozjpeg

mkdir build
mkdir install

cd build

cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/mozjpeg/install \
  -DWITH_JPEG8=ON \
  -DPNG_SUPPORTED=OFF

make -j$(nproc)
sudo make install

#cmake --build . -j$(nproc)
#sudo cmake --install build