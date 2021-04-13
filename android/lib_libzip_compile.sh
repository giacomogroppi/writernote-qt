#!/bin/sh

rm libzip-android-0.9.3/
rm 0.9.3.zip

wget https://codeload.github.com/julienr/libzip-android/zip/refs/tags/0.9.3
#wget https://github.com/dec1/libzip-android/archive/libzip_1.2.0.zip
unzip v1.0.1.zip -d libzip

cd libzip/libzip-android-1.0.1

##~/Android/Sdk/ndk/22.0.7026061/ndk-build NDK_PROJECT_PATH=./ $@

#set the path to the android ndk here:
#export ANDROID_NDK=/home/giacomo/Android/Sdk/ndk/22.0.7026061/

#export PATH=${ANDROID_NDK}:$PATH

#export NDK_PROJECT_PATH=./

# Pass various options here. Can also be specified in Application.mk, but those will be overridden by any also passed here
# see https://developer.android.com/ndk/guides/application_mk.html
# and https://developer.android.com/ndk/guides/ndk-build.html

#ndk-build APP_STL=gnustl_shared     \
#          APP_PLATFORM=android-16    \
#          APP_ABI="armeabi armeabi-v7a"     \
#          NDK_TOOLCHAIN_VERSION=5   \
#          NDK_PROJECT_PATH=./       \
#          $@

#ndk-build clean APP_STL=gnustl_shared
