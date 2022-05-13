pacman --noconfirm -Sy

pacman --noconfirm -S git
pacman --noconfirm -S mingw-w64-x86_64-toolchain \
          mingw-w64-x86_64-cmake \
          patch \
          mingw-w64-x86_64-cppunit \
          make \

pacman --noconfirm -S mingw-w64-x86_64-nsis

pacman --noconfirm -S mingw-w64-x86_64-qt6-base \
          mingw-w64-x86_64-libzip \
          mingw-w64-x86_64-glib2

pacman --noconfirm -S mingw-w64-x86_64-poppler-qt6
pacman --noconfirm -S mingw-w64-x86_64-ninja
