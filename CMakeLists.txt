cmake_minimum_required(VERSION 3.5)

project(writernote LANGUAGES CXX)
include(FindPkgConfig)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE AND NOT MINGW)
    set(LINUX TRUE)
endif()

if(MSVC OR MSYS OR MINGW)
    set(WIN TRUE)
endif()

set(QT_VERSION_MAJOR 6)
message("-- Version qt: " ${QT_VERSION_MAJOR})

file(GLOB PROJECT_MOC
	writernote.qrc
    language.qrc
)

file(GLOB_RECURSE PROJECT_UI
    ${PROJECT_SOURCE_DIR}/src/*.ui
)

file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
    src/*.h
    updater/launch_updater/launch_updater.*
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

else()
    message("Pdf Enable")
    add_definitions(-DPDFSUPPORT)
    target_include_directories(${PROJECT_NAME} PRIVATE /usr/include/poppler/qt5) 
endif()

if(APPLE)
add_definitions(-DMACOS)
endif()

function(setordefault varname variable default)
  if ("${variable} " STREQUAL " ")
    set(${varname} ${default} PARENT_SCOPE)
  else()
    set(${varname} ${variable} PARENT_SCOPE)
  endif()
endfunction()

option(SNAP "Snap package" OFF)
option(PDFSUPPORT "Pdf support" ON)
option(MACOS_BUILD "Set to on if we are building in action" OFF)
option(VERSION_SNAPCRAFT "Version" "")
option(FLATPAK_BUILD "build for flatpak" OFF)
option(DEBUG_CORE "Debug core class" OFF)
option(ANDROID "Android build" OFF)

if(FLATPAK_BUILD)
    set(WRITERNOTE_APPL_DIR "" CACHE PATH "Folder to install .desktop files to. If empty, the default is CMAKE_INSTALL_PREFIX/share/applications")
    set(WRITERNOTE_METAINFO_DIR "" CACHE PATH "Folder to install .appdata.xml files to. If empty, the default is CMAKE_INSTALL_PREFIX/share/metainfo")

    setordefault(WRITERNOTE_APPL "${WRITERNOTE_APPL_DIR}" ${CMAKE_INSTALL_PREFIX}/share/applications)
    setordefault(WRITERNOTE_METAINFO_DIR "${WRITERNOTE_METAINFO_DIR}" ${CMAKE_INSTALL_PREFIX}/share/metainfo)
endif()

if(NOT ${VERSION_SNAPCRAFT} STREQUAL "")
    add_definitions(-DVERSION_SNAPCRAFT=${VERSION_SNAPCRAFT})
    message("-- Add version " ${VERSION_SNAPCRAFT})
endif()

if(SNAP)
    message("-- Snap support")
    add_definitions(-DSNAP)  
endif()

if(DEBUG_CORE)
    SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-DDEBUG_CORE")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if(LINUX AND NOT FLATPAK_BUILD AND NOT SNAP)
    
    if(NOT "$ENV{QT_VER_WRITERNOTE}" STREQUAL "")
        message("-- QT_VER_WRITERNOTE set")
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "~/Qt/$ENV{QT_VER_WRITERNOTE}/gcc_64/lib/cmake")
    else()
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "~/Qt/6.4.0/gcc_64/lib/cmake")
    endif()
    
    message("-- search qt in " ${CMAKE_PREFIX_PATH})
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}Multimedia REQUIRED )
find_package(Qt${QT_VERSION_MAJOR}MultimediaWidgets REQUIRED )
find_package(Qt${QT_VERSION_MAJOR}PrintSupport REQUIRED)

if(ANDROID)
    find_package(Qt${QT_VERSION_MAJOR} 
        REQUIRED COMPONENTS
        AndroidExtras
    )
endif()

if(NOT APPLE)
    qt_add_executable(${PROJECT_NAME} ${PROJECT_MOC} ${PROJECT_SOURCES} ${PROJECT_UI})
else()
    qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_MOC} ${PROJECT_SOURCES} ${PROJECT_UI} src/touch/TabletPenMethod.cpp src/touch/TabletPenMethod.h)
endif()

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	AUTOUIC TRUE
	AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/src"
	AUTOMOC TRUE
	AUTORCC TRUE
	AUTOGEN_SOURCE_GROUP "Qt Files"
)

if(PDFSUPPORT)
    add_definitions(-DPDFSUPPORT)
    message("-- Add pdf support")
    
    if(SNAP)        
        include_directories(/usr/local/include/poppler/qt6)
        if(EXISTS /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler.so)
            target_link_libraries(${PROJECT_NAME} PRIVATE /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler-qt6.so.3)
            target_link_libraries(${PROJECT_NAME} PRIVATE /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler.so)
            target_link_libraries(${PROJECT_NAME} PRIVATE /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler.so.121.0.0)
            target_link_libraries(${PROJECT_NAME} PRIVATE /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler-qt6.so)
            target_link_libraries(${PROJECT_NAME} PRIVATE /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler-qt6.so.3.2.0)
            target_link_libraries(${PROJECT_NAME} PRIVATE /build/writernote/prime/usr/lib/x86_64-linux-gnu/libpoppler.so.121)
        else()
            target_link_libraries(${PROJECT_NAME} PRIVATE /root/prime/usr/lib/x86_64-linux-gnu/libpoppler-qt6.so.3)
            target_link_libraries(${PROJECT_NAME} PRIVATE /root/prime/usr/lib/x86_64-linux-gnu/libpoppler.so)
            target_link_libraries(${PROJECT_NAME} PRIVATE /root/prime/usr/lib/x86_64-linux-gnu/libpoppler.so.121.0.0)
            target_link_libraries(${PROJECT_NAME} PRIVATE /root/prime/usr/lib/x86_64-linux-gnu/libpoppler-qt6.so)
            target_link_libraries(${PROJECT_NAME} PRIVATE /root/prime/usr/lib/x86_64-linux-gnu/libpoppler-qt6.so.3.2.0)
            target_link_libraries(${PROJECT_NAME} PRIVATE /root/prime/usr/lib/x86_64-linux-gnu/libpoppler.so.121)
        endif()

    elseif(FLATPAK_BUILD)        
        include_directories(/app/include/poppler/qt6)
        target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libpoppler.so.121.0.0)
        target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libpoppler.so.121)
        target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libpoppler.so)
        
        target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libpoppler-qt6.so.3.2.0)
        target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libpoppler-qt6.so.3)
        target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libpoppler-qt6.so)

    elseif(LINUX)        
        message("project source " ${PROJECT_SOURCE_DIR})
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/poppler/qt6/src/)
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/poppler/build/qt6/src)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/install/libpoppler-qt6.so)

    elseif(WIN)
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/poppler/include/poppler/qt6)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/lib/libpoppler-qt6.dll.a)
    
    elseif(MACOS_BUILD)
        set(POPPLER_DIR ${PROJECT_SOURCE_DIR}/3rdparty/poppler/include/poppler/qt6)

        message("-- Use " ${POPPLER_DIR} " as poppler include path")

        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/lib/libpoppler.a)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/lib/libpoppler-qt6.a)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/libpng/install/lib/libpng.a)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/zlib-ng/distrib/lib/libz.a )
        include_directories(${POPPLER_DIR})
    endif()
endif() #PDFSUPPORT

if(MACOS_BUILD)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/libzip/distrib/include/)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/libzip/distrib/lib/libzip.a)
elseif(WIN)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/libzip/install/include/)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/libzip/install/lib/libzip.a)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/zlib-ng/install/lib/libzlib.a)
endif()


SET(QT_USE_QTMULTIMEDIA TRUE)
SET(QT_USE_QTMULTIMEDIAWIDGETS TRUE)
set(QT_USE_PRINTSUPPORT TRUE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUGINFO -O0")

add_definitions(-DALL_VERSION)

include_directories(src/)

SET (CMAKE_CXX_FLAGS_RELEASE "-Wno-deprecated-declarations -O3")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

if(FLATPAK_BUILD)
    include_directories(/app/include/)
    include_directories(/app/7app/include/)
    target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libzip.so)
    target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libzip.so.5.4)
    target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libzip.so.5)
elseif(NOT FLATPAK_BUILD AND LINUX)
    set(LIBZIP_PATH /usr/lib/x86_64-linux-gnu/libzip.so)
    if(EXISTS ${LIBZIP_PATH})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBZIP_PATH})
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib/libzip.so)
    endif()
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    #set up poppler-qt6
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib/x86_64-linux-gnu/libpoppler-qt5.so)
endif()

if(NOT APPLE)
install (TARGETS writernote RUNTIME DESTINATION ${PROJECT_BINARY_BIN})
endif()

if(FLATPAK_BUILD)
    install(FILES ${PROJECT_SOURCE_DIR}/flatpak/com.github.giacomogroppi.writernote-qt.desktop DESTINATION      ${WRITERNOTE_APPL})
    install(FILES ${PROJECT_SOURCE_DIR}/flatpak/com.github.giacomogroppi.writernote-qt.appdata.xml DESTINATION  ${WRITERNOTE_METAINFO_DIR})
    install(FILES ${PROJECT_SOURCE_DIR}/flatpak/com.github.giacomogroppi.writernote-qt.svg DESTINATION          ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps/)
    install(FILES ${PROJECT_SOURCE_DIR}/flatpak/w-writer.desktop DESTINATION                                    ${CMAKE_INSTALL_PREFIX}/share/mimelnk/application)
    install(FILES ${PROJECT_SOURCE_DIR}/flatpak/com.github.giacomogroppi.writernote-qt.xml DESTINATION          ${CMAKE_INSTALL_PREFIX}/share/mime/packages)

    set(ALL_SIZE 128x128 32x32 48x48 512x512 64x64)
    foreach(size IN LISTS ALL_SIZE)
        message(STATUS "size = ${size}")
        install(FILES ${PROJECT_SOURCE_DIR}/flatpak/com.github.giacomogroppi.writernote-qt-${size}.png
                DESTINATION ${CMAKE_INSTALL_PREFIXCMAKE_INSTALL_PREFIX}/share/icons/hicolor/${size}/com.github.giacomogroppi.writernote-qt.png 
                RENAME com.github.giacomogroppi.writernote-qt.png )
    endforeach()

endif()
