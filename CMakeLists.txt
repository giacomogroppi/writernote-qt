cmake_minimum_required(VERSION 3.5)

project(writernote LANGUAGES CXX)
include(FindPkgConfig)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE AND NOT MINGW)
    set(LINUX TRUE)
endif()

if(MSVC OR MSYS OR MINGW)
    set(WIN TRUE)
endif()

file(GLOB PROJECT_MOC
	writernote.qrc
    language.qrc
)

file(GLOB_RECURSE PROJECT_UI
    ${PROJECT_SOURCE_DIR}/src/*.ui
)

set (A_DIR_ROOT ${CMAKE_SOURCE_DIR})

set(PROJECT_SOURCES
        src/frompdf/metadata.cpp
        src/frompdf/convertimg.cpp
        src/frompdf/frompdf.cpp
        src/sheet/style_struct.cpp
        src/cloud/cloud_controll.cpp
        src/cloud/struct_file.cpp
        src/cloud/cloud_ui.cpp
        src/cloud/on_select.cpp
        src/cloud/utils/downloadfile.cpp
        src/cloud/utils/toencrypt.cpp
        src/cloud/struct_user.cpp
        src/audioplay/audioplay.cpp
        src/lastedit/redoundo.cpp
        src/datawrite/write/savefile_savefile.cpp
        src/datawrite/write/saveArray.cpp
        src/datawrite/write/salvabinario.cpp
        src/audiorecord/audiorecord.cpp
        src/utils/removenotwriternote.cpp
        src/utils/checkpassword/checkpassword.cpp
        src/utils/copy_text.cpp
        src/utils/random/random.cpp
        src/utils/mostra_explorer/mostra_explorer.cpp
        src/utils/lastModification/lastmodification.cpp
        src/utils/path/get_path.cpp
        src/utils/path/pathfile.cpp
        src/utils/permission/permission.cpp
        src/utils/remove_key/remove_key.cpp
        src/utils/time/current_time.cpp
        src/utils/Optional.cpp
        src/utils/color/color_chooser.cpp
        src/utils/get_only_name/get_only_name.cpp
        src/utils/threadcount.cpp
        src/utils/get_file_dir/get_file_dir.cpp
        src/testing/memtest.cpp
        src/testing/testingcore.cpp
        src/testing/testingzip.cpp
        src/file/File.cpp
        src/file/FileManager.cpp
        src/file/Directory.cpp
        src/preview/preview.cpp
        src/restore_file/restore_file_critic.cpp
        src/touch/pen/PenMethod.cpp
        src/touch/pen/Pen.cpp
        src/touch/square/Square.cpp
        src/touch/square/SquareMethod.cpp
        src/touch/dataTouch/page/PageFile.cpp
        src/touch/dataTouch/page/Page.cpp
        src/touch/dataTouch/datastruct/utils_datastruct.cpp
        src/touch/dataTouch/datastruct/DataStruct.cpp
        src/touch/dataTouch/datastruct/controllForRepositioning.cpp
        src/touch/dataTouch/datastruct/changeId.cpp
        src/touch/dataTouch/datastruct/triggerNewView.cpp
        src/touch/dataTouch/datastruct/removePage.cpp
        src/touch/dataTouch/stroke/StrokeCircle.cpp
        src/touch/dataTouch/stroke/StrokeLine.cpp
        src/touch/dataTouch/stroke/StrokeRectGenerator.cpp
        src/touch/dataTouch/stroke/StrokeForPage.cpp
        src/touch/dataTouch/stroke/StrokeNormalFile.cpp
        src/touch/dataTouch/stroke/StrokeLineGenerator.cpp
        src/touch/dataTouch/stroke/StrokeRect.cpp
        src/touch/dataTouch/stroke/StrokePre.cpp
        src/touch/dataTouch/stroke/StrokeNormal.cpp
        src/touch/dataTouch/stroke/StrokeCircleGenerator.cpp
        src/touch/dataTouch/stroke/stroke_complex_data.cpp
        src/touch/dataTouch/stroke/StrokeProp.cpp
        src/touch/dataTouch/stroke/Stroke.cpp
        src/touch/dataTouch/stroke/stroke_file.cpp
        src/touch/object_finder/model_finder/model_finder.cpp
        src/touch/object_finder/ObjectFinder.cpp
        src/touch/rubber/RubberMethod.cpp
        src/touch/rubber/Rubber.cpp
        src/touch/TabletController.cpp
        src/touch/TabletUtils.cpp
        src/touch/multi_thread_data.cpp
        src/touch/copy_cut/copy_cut_selection.cpp
        src/touch/highlighter/HighligterMethod.cpp
        src/touch/highlighter/Highligter.cpp
        src/touch/tools/Tools.cpp
        src/touch/tools/Scrollable.cpp
        src/touch/tools/InsertTools.cpp
        src/touch/laser/Laser.cpp
        src/touch/laser/LaserMethod.cpp
        src/touch/TabletPenMethod.cpp
        src/FileContainer/FileContainer.cpp
        src/FileContainer/FileWriterMulti.cpp
        src/FileContainer/WZipWriter.cpp
        src/FileContainer/MemWritable.cpp
        src/FileContainer/WZipReaderSingle.cpp
        src/FileContainer/WZipWriterMulti.cpp
        src/FileContainer/WZipWriterSingle.cpp
        src/FileContainer/FileInstance.cpp
        src/FileContainer/WZipCommon.cpp
        src/FileContainer/FileReader.cpp
        src/FileContainer/WZip.cpp
        src/FileContainer/MemReadable.cpp
        src/FileContainer/FileWriter.cpp
        src/core/RectF.cpp
        src/core/AtomicSafe.cpp
        src/core/WVector.cpp
        src/core/WFile.cpp
        src/core/WRecursiveLock.cpp
        src/core/WPixmap.cpp
        src/core/WPair.cpp
        src/core/WReadZip.cpp
        src/core/WSemaphore.cpp
        src/core/ExactNumber/ExactInteger.cpp
        src/core/ExactNumber/ExactNumber.cpp
        src/core/WRect.cpp
        src/core/WMap.cpp
        src/core/WSize.cpp
        src/core/WList.cpp
        src/core/multiarray.cpp
        src/core/WImage.cpp
        src/core/WDir.cpp
        src/core/WListFast.cpp
        src/core/pointer/UniquePtr.cpp
        src/core/pointer/SharedPtr.cpp
        src/core/WListThreadSafe.cpp
        src/core/WRgb.cpp
        src/core/WMutexLocker.cpp
        src/core/ByteArray/WByteArray.cpp
        src/core/WPainter/WPainter.cpp
        src/core/WMultipleMutex.cpp
        src/core/WLine.cpp
        src/core/WAbstractList.cpp
        src/core/Point.cpp
        src/core/PointF.cpp
        src/core/core.cpp
        src/core/WVariant.cpp
        src/core/WPen.cpp
        src/core/String/WString.cpp
        src/core/WOptionSettings.cpp
        src/pdf/translate.cpp
        src/pdf/topdf.cpp
        src/VersionFileController.cpp
        src/images/fromimage.cpp
        src/images/ImageDrawable.cpp
        src/dataread/readlistarray.cpp
        src/dataread/xmlstruct.cpp
        src/dataread/loadbinario.cpp
        src/dataread/loadfile.cpp
        src/dataread/old_version/ver_6/load_file_ver_6.cpp
        src/dataread/old_version/ver_5/load_file_ver_5.cpp
        src/dataread/old_version/ver_2/load_file_ver_2.cpp
        src/dataread/old_version/ver_7/load_file_ver_7.cpp
        src/dataread/old_version/ver_8/load_file_ver_8.cpp
        src/dataread/old_version/ver_3/load_file_ver_3.cpp
        src/dataread/old_version/ver_4/load_file_ver_4.cpp
        src/dataread/old_loadbinario/old_version_load_binario.cpp
        src/windows/mostra_finestra_i.cpp
        src/windows/updatecheck.cpp
        src/log/log_ui/log_ui.cpp
        src/Scheduler/WTask.cpp
        src/Scheduler/WObject.cpp
        src/Scheduler/WPool.cpp
        src/Scheduler/WTimer.cpp
        src/Scheduler/Scheduler.cpp
        src/currenttitle/checksimilecopybook.cpp
        src/currenttitle/merge_currenttitle/merge_currenttitle.cpp
        src/currenttitle/document.cpp
)

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
option(PDFSUPPORT "Pdf support" OFF)
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

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

if(APPLE)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/macos-build/Info.plist
    )
else()
endif()

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

    elseif(WIN)
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/poppler/include/poppler/qt6)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/lib/libpoppler-qt6.dll.a)
    
    elseif(MACOS_BUILD OR APPLE)
        set(POPPLER_DIR ${PROJECT_SOURCE_DIR}/3rdparty/poppler/include/poppler/qt6)

        message("-- Use " ${POPPLER_DIR} " as poppler include path")

        #target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/lib/libpoppler.a)
        #target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/poppler/lib/libpoppler-qt6.a)
        #target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/libpng/install/lib/libpng.a)
        #target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/zlib-ng/distrib/lib/libz.a )
        include_directories(${POPPLER_DIR})
    endif()
endif() #PDFSUPPORT

if(MACOS_BUILD OR APPLE)
    include_directories(/opt/homebrew/Cellar/libzip/1.9.2/include/)
    #target_link_libraries(${PROJECT_NAME} PRIVATE /opt/homebrew/Cellar/libzip/1.9.2/lib/libzip.dylib)
elseif(WIN)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/libzip/install/include/)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/libzip/install/lib/libzip.a)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/zlib-ng/install/lib/libzlib.a)
endif()

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUGINFO -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

add_definitions(-DALL_VERSION)

include_directories(src/)

SET (CMAKE_CXX_FLAGS_RELEASE "-Wno-deprecated-declarations -O3")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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
