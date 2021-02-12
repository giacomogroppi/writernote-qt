cd libzip/libzip-android-1.0.1/

#set the path to the android ndk here:
export ANDROID_NDK=/home/giacomo/Android/Sdk/ndk/22.0.7026061/

export PATH=${ANDROID_NDK}:$PATH

export NDK_PROJECT_PATH=./

# Pass various options here. Can also be specified in Application.mk, but those will be overridden by any also passed here
# see https://developer.android.com/ndk/guides/application_mk.html
# and https://developer.android.com/ndk/guides/ndk-build.html

ndk-build APP_STL=c++_shared     \
          APP_PLATFORM=android-16    \
          APP_ABI="armeabi-v7a"     \
          NDK_TOOLCHAIN_VERSION=5   \
          NDK_PROJECT_PATH=./       \
          $@

#ndk-build clean APP_STL=gnustl_shared
