sh clear_poppler.sh
cd poppler

mkdir build
mkdir install

# qt from qt installer
if [ ! -z QT_VER_WRITERNOTE ]
then
    QT_VER_WRITERNOTE=6.3.0
fi

echo "Qt version set to: ${QT_VER_WRITERNOTE}" 


Qt6_POS=~/Qt/${QT_VER_WRITERNOTE}/gcc_64/lib/cmake/

cd build
cmake .. -DCMAKE_INSTALL_LIBDIR=~/writernote-qt/3rdparty/poppler/install \
    -DCMAKE_INSTALL_INCLUDEDIR=~/writernote-qt/3rdparty/poppler/install \
    -DENABLE_GLIB=OFF \
    -DENABLE_GTK_DOC=OFF \
    -DENABLE_QT5=OFF \
    -DENABLE_QT6=ON \
    -DENABLE_BOOST=OFF \
    -DBUILD_QT6_TESTS=OFF \
    -DBUILD_QT5_TESTS=OFF \
    -DENABLE_UTILS=OFF \
    -DENABLE_CMS=lcms2 \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_PREFIX_PATH=$Qt6_POS

make -j16 
sudo make install
