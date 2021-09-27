
# Author: Declan Moran
# www.silverglint.com 
# Thanks to damaex (https://github.com/damaex), for significant contributions

cd libzip/android

$ANDROID_NDK_ROOT=$2

$INSTALL_DIR=install
$BUILD_DIR=build
$START_DIR=$(pwd)

rm -rf $INSTALL_DIR
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR 

want_shared=$1

&'cmake.exe -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake -DCMAKE_INSTALL_PREFIX:PATH=../../${INSTALL_DIR}/x86 -DANDROID_ABI=x86	-DENABLE_OPENSSL:BOOL=OFF -DENABLE_COMMONCRYPTO:BOOL=OFF -DENABLE_GNUTLS:BOOL=OFF -DENABLE_MBEDTLS:BOOL=OFF -DENABLE_OPENSSL:BOOL=OFF -DENABLE_WINDOWS_CRYPTO:BOOL=OFF -DBUILD_TOOLS:BOOL=OFF -DBUILD_REGRESS:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DBUILD_SHARED_LIBS:BOOL=$want_shared -DBUILD_DOC:BOOL=OFF -DANDROID_TOOLCHAIN=clang  cmake -H.. -B$BUILD_DIR/x86'

#run make with all system threads and install
cd $BUILD_DIR/x86
make install -j$(nproc)
cd $START_DIR
