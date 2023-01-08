cd $PREFIX_LIB

sudo rm -r freetype-2.12.1.tar.xz
sudo rm -r freetype

wget https://download.savannah.gnu.org/releases/freetype/freetype-2.12.1.tar.xz
tar -xf freetype-2.12.1.tar.xz
rm freetype-2.12.1.tar.xz
mv freetype-2.12.1 freetype
cd freetype

mkdir build
mkdir install
          
cd build
          
cmake .. -DCMAKE_INSTALL_PREFIX=$PREFIX_LIB/freetype/install
make -j8
sudo make install