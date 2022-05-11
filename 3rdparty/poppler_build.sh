sh clear_poppler.sh
cd poppler

mkdir build
mkdir install

# qt from repository
#Qt5_POS=/usr/lib/x86_64-linux-gnu/cmake/Qt5
#Qt6_POS=/usr/lib/x86_64-linux-gnu/cmake/Qt6

# qt from qt installer
Qt6_POS=~/Qt/6.3.0/gcc_64/lib/cmake/Qt6/

cd build
cmake .. -DCMAKE_INSTALL_LIBDIR=/home/giacomo/writernote-qt/3rdparty/poppler/install \
    -DCMAKE_INSTALL_INCLUDEDIR=/home/giacomo/writernote-qt/3rdparty/poppler/install \
    -DENABLE_GLIB=OFF \
    -DENABLE_GTK_DOC=OFF \
    -DENABLE_QT5=OFF \
    -DENABLE_QT6=ON \
    -DENABLE_BOOST=OFF \
    -DBUILD_QT6_TESTS=OFF \
    -DBUILD_QT5_TESTS=OFF \
    -DENABLE_UTILS=OFF \
    -DENABLE_CMS=lcms2 \
    -DQt6_DIR=${Qt6_POS}/Qt6Config.cmake \
    -DQt5_DIR=${Qt5_POS}/Qt5Config.cmake

make -j16 
sudo make install