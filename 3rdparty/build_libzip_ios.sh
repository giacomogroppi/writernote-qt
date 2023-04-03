export CC=clang;
export CROSS_TOP=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer
export CROSS_SDK=iPhoneOS.sdk
export PATH="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin:$PATH"

export IOS_TOOLCHAIN_FILE=$(pwd)/ios-cmake/ios.toolchain.cmake

set -e

export CURDIR=$PWD

cd libzip

export INSTALL_DIR=$(pwd)/install/platform/ios/libzip/Release/ios
mkdir -p build
cd build

declare -a PLATFORMS=("OS" "OS64" "SIMULATOR" "SIMULATOR64")
FLATFORM="OS64"

cmake .. \
    -G "Xcode" \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DENABLE_BITCODE=0 \
    -DCMAKE_TOOLCHAIN_FILE=$IOS_TOOLCHAIN_FILE \
    -DPLATFORM=${PLATFORMS} \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_LZMA=FALSE \
    -DENABLE_OPENSSL=FALSE \
    -DENABLE_MBEDTLS=FALSE \
    -DENABLE_GNUTLS=FALSE \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_TOOLS=OFF \
    -DBUILD_REGRESS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_DOC=OFF

cmake --build . --config Release --target install
