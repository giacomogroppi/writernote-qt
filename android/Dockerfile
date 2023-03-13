FROM ubuntu:22.04

# Installa pacchetti necessari
RUN apt-get update && apt-get install -y \
    build-essential \
    libgl1-mesa-dev \
    libxcb-xinerama0-dev \
    libx11-xcb-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libx11-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxrender-dev \
    libxcb1-dev \
    libxcb-glx0-dev \
    libxcb-keysyms1-dev \
    libxcb-image0-dev \
    libxcb-shm0-dev \
    libxcb-icccm4-dev \
    libxcb-sync-dev \
    libxcb-xfixes0-dev \
    libxcb-shape0-dev \
    libxcb-randr0-dev \
    libxcb-render-util0-dev \
    libxcb-xinerama0-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libwayland-dev \
    libwayland-egl-backend-dev \
    libepoxy-dev \
    libpulse-dev \
    libicu-dev \
    libsqlite3-dev \
    libpq-dev \
    libssl-dev \
    libasound2-dev \
    python3 \
    python3-pip \
    git \
    cmake \
    ninja-build \
    openjdk-8-jdk \
    && rm -rf /var/lib/apt/lists/*

# settaggio di skd
RUN apt-get update && apt-get -y install android-sdk \
    sdkmanager

RUN find / -name "sdkmanager" && echo ""

ENV SDK_VERSION=31

RUN sdkmanager "ndk;23.1.7779620"
RUN sdkmanager "platforms;android-${SDK_VERSION}" "platform-tools" "build-tools;${SDK_VERSION}.0.0"

# Imposta variabili d'ambiente per Qt
ENV QT_VERSION=6.4.2
ENV QT_PATH=/opt/Qt${QT_VERSION}
ENV QT_DIR=${QT_PATH}/${QT_VERSION}/gcc_64
ENV QT_POSITION=${HOME}qt_tmp

# è la posizione dell'intallazione di qt dell'host
ENV QT_PLAT=/opt/build_cache_qt_plat
ENV QT_PLAT_INSTALLATION=/opt/tmp_Qt${QT_VERSION}/

RUN mkdir ${QT_POSITION}
RUN mkdir ${QT_PLAT}

RUN git clone -b v${QT_VERSION} https://code.qt.io/qt/qt5.git ${QT_PLAT} \
    && cd ${QT_PLAT} \
    && perl init-repository --module-subset=qtbase,qtsvg,qtmultimedia,qtshadertools \  
    && mkdir -p $QT_PLAT_INSTALLATION \
    && cmake . \
        -DCMAKE_INSTALL_PREFIX=$QT_PLAT_INSTALLATION \
        -GNinja \
    && cmake --build . --parallel \
    && cmake --install .

ENV NDK_VERSION=23.1.7779620

ENV JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/
ENV ANDROID_NDK_ROOT=/opt/android-sdk/ndk/${NDK_VERSION}
ENV ANDROID_SDK_ROOT=/opt/android-sdk/build-tools/${SDK_VERSION}.0.0

ENV CMAKE_CXX_COMPILER=${ANDROID_NDK_ROOT}/toolchains/llvm/prebuilt/linux-x86_64/clang++
ENV CMAKE_C_COMPILER=${ANDROID_NDK_ROOT}/toolchains/llvm/prebuilt/linux-x86_64/clang

#download qt
RUN git clone -b v${QT_VERSION} https://code.qt.io/qt/qt5.git ${QT_POSITION} \
    && cd ${QT_POSITION} \
    && perl init-repository --module-subset=qtbase,qtsvg,qtmultimedia,qtshadertools

RUN sdkmanager "platforms;android-${SDK_VERSION}"
ENV ANDROID_TARGET_PLATFORM=arm64-v8a

#build & install qt
RUN cd ${QT_POSITION} \
    && cmake . \
        -GNinja \
        -DCMAKE_SYSTEM_NAME="Android" \
        -DCMAKE_INSTALL_PREFIX=${QT_PATH} \
        -DANDROID_TOOLCHAIN_NAME="aarch64-linux-android-clang" \
        -DCMAKE_ANDROID_NDK=${ANDROID_NDK_ROOT} \
        -DANDROID_SDK_ROOT=${ANDROID_SDK_ROOT} \
        -DCMAKE_ANDROID_NDK=${ANDROID_NDK_ROOT} \
        -DANDROID_ABI=${ANDROID_TARGET_PLATFORM} \
        -DANDROID_PLATFORM=${SDK_VERSION} \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
        -DQT_HOST_PATH=${QT_PLAT_INSTALLATION} \
    && cmake --build . --parallel \
    && cmake --install .

ENV LIBZIP_PATH=/opt/libzip
ENV LIBZIP_INSTALL=${LIBZIP_PATH}/install
ENV LIBZIP_BUILD=${LIBZIP_PATH}/build


# build libzip
RUN git clone -b v1.7.3 https://github.com/nih-at/libzip ${LIBZIP_PATH} \
    && cd ${LIBZIP_PATH} \
    && cmake . \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
		-DCMAKE_INSTALL_PREFIX:PATH=../../${LIBZIP_INSTALL}/${ANDROID_TARGET_PLATFORM} \
		-DANDROID_ABI=${ANDROID_TARGET_PLATFORM} \
		-DENABLE_OPENSSL:BOOL=OFF \
		-DENABLE_COMMONCRYPTO:BOOL=OFF \
		-DENABLE_GNUTLS:BOOL=OFF \
		-DENABLE_MBEDTLS:BOOL=OFF \
		-DENABLE_OPENSSL:BOOL=OFF \
		-DENABLE_WINDOWS_CRYPTO:BOOL=OFF \
		-DBUILD_TOOLS:BOOL=OFF \
		-DBUILD_REGRESS:BOOL=OFF \
		-DBUILD_EXAMPLES:BOOL=OFF \
		-DBUILD_SHARED_LIBS:BOOL=FALSE \
		-DBUILD_DOC:BOOL=OFF \
        -DANDROID_PLATFORM=${SDK_VERSION} \
		-DANDROID_TOOLCHAIN=clang \
    && cmake -B${LIBZIP_BUILD} \
    && cd ${LIBZIP_BUILD} \
    && make install -j$(nproc --all)


ENV FREETYPE_PATH=/opt/freetype
ENV FREETYPE_INSTALL=${FREETYPE_PATH}/install
ENV FREETYPE_BUILD=${FREETYPE_PATH}/build

# freetype
RUN apt -y install wget
RUN wget https://download.savannah.gnu.org/releases/freetype/freetype-2.13.0.tar.gz \
    && tar -xvzf freetype-2.13.0.tar.gz \
    && mv freetype-2.13.0 ${FREETYPE_PATH} \
    && cd ${FREETYPE_PATH} \
    && cmake . \
        -G Ninja \
        -B ${FREETYPE_BUILD} \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
        -DCMAKE_INSTALL_PREFIX=${FREETYPE_INSTALL} \
        -DANDROID_ABI=${ANDROID_TARGET_PLATFORM} \
        -DBUILD_SHARED_LIBS=FALSE \
        -DANDROID_TOOLCHAIN=clang \
        -DANDROID_PLATFORM=${SDK_VERSION} \
    && cd ${FREETYPE_BUILD} \
    && ninja \
    && ninja install

ENV OPENJPEG_PATH=/opt/openjpeg
ENV OPENJPEG_INSTALL=${OPENJPEG_PATH}/install
ENV OPENJPEG_BUILD=${OPENJPEG_PATH}/build

RUN git clone -b v2.5.0 https://github.com/uclouvain/openjpeg ${OPENJPEG_PATH} \
    && cd ${OPENJPEG_PATH} \
    && cmake . \
        -G Ninja \
        -B ${OPENJPEG_BUILD} \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
        -DCMAKE_INSTALL_PREFIX=${OPENJPEG_INSTALL} \
        -DANDROID_ABI=${ANDROID_TARGET_PLATFORM} \
        -DBUILD_SHARED_LIBS=FALSE \
        -DANDROID_TOOLCHAIN=clang \
        -DANDROID_PLATFORM=${SDK_VERSION} \
    && cd ${OPENJPEG_BUILD} \
    && ninja \
    && ninja install

ENV POPPLER_BUILD=${POPPLER_PATH}/build
ENV POPPLER_INSTALL=${POPPLER_PATH}/install
ENV POPPLER_PATH=/opt/poppler

# build poppler
RUN git clone -b poppler-23.03.0 https://gitlab.freedesktop.org/poppler/poppler ${POPPLER_PATH} \
    && cd ${POPPLER_PATH} \
    && export Qt6_DIR=${QT_PATH}/lib/cmake/Qt6 \
    && cmake . \
        -G Ninja \
        -B ${POPPLER_BUILD} \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
        -DCMAKE_INSTALL_PREFIX:PATH=${POPPLER_INSTALL} \
        -DANDROID_ABI=${ANDROID_TARGET_PLATFORM} \
        -DBUILD_SHARED_LIBS=FALSE \
        -DANDROID_PLATFORM=${SDK_VERSION} \
        -DANDROID_TOOLCHAIN=clang \
        -DFREETYPE_LIBRARY=${FREETYPE_INSTALL}/lib/libfreetype.a \
        -DFREETYPE_INCLUDE_DIRS=${FREETYPE_INSTALL}/include/freetype2 \
        -DENABLE_DCTDECODER=unmaintained \
        -DENABLE_BOOST=OFF \
        -DOpenJPEG_DIR=${OPENJPEG_INSTALL}/lib/openjpeg-2.5/ \
        -DENABLE_QT6=OFF \
        -DENABLE_QT5=OFF \
    && cd ${POPPLER_BUILD} \
    && ninja \
    && ninja install
