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


file(GLOB PROJECT_UI
    src/audiosetting/audioqualityoption.ui
    src/currenttitle/option/option_copybook.ui
    src/last_open/element/element_ui.ui
    src/last_open/last_open.ui 
    src/last_open/option/option_last_open_ui.ui 
    src/log/log_ui/log_ui.ui 
    src/mainwindow.ui 
    src/restore_file/ui/setting_restore_ui.ui 
    src/restore_file/ui_restore/restore.ui 
    src/sheet/dialog_sheet.ui 
    src/sheet/fast-sheet/fast_sheet_ui.ui 
    src/touch/highlighter/highlighter.ui 
    src/touch/pen/pen_ui.ui 
    src/touch/rubber/rubber_ui.ui 
    src/touch/scrollKinetic/ui_scroll/scroll.ui 
    src/touch/text/text_ui/text_ui.ui 
    src/touch/text/text_widgets/text_widgets.ui 
    src/utils/bug_repo/bug_repo_ui.ui 
    src/utils/default_location/audio_default_location.ui 
    src/utils/default_option/default_option_ui.ui 
    src/utils/extract_audio/extract_audio_ui.ui 
    src/utils/make_default/make_default_ui.ui 
    src/utils/progress_bar/progress_bar_ui.ui 
    src/utils/retry/retry_ui.ui 
    src/videocompress/videocompress_ui.ui 
    src/windows/showmessageupdate.ui
    src/utils/choosepage.ui
    src/preview_page_widget/preview_page_container.ui
    src/preview_page_widget/preview_page_widget.ui
    src/preview_page_widget/preview_page_item.ui
    src/ui/toolbar.ui
)


file(GLOB PROJECT_SOURCES
    src/touch/datastruct/utils_datastruct.cpp
    src/dataread/xmlstruct.h
    src/dataread/readlistarray.h
    src/dataread/load_from_file.h
    src/preview_page_widget/preview_page_item.h
    src/preview_page_widget/preview_page_widget.h
    src/preview_page_widget/preview_page_container.h
    src/preview_page_widget/list_options.h
    src/pdf/topdf.h
    src/mainwindow.h
    src/savecopybook.h
    src/datawrite/renamefile_f_zip.h
    src/datawrite/savefile.h
    src/datawrite/qfilechoose.h
    src/datawrite/source_read_ext.h
    src/log/log_ui/log_ui.h
    src/touch/paintevent/paint.h
    src/touch/object_finder/model/model.h
    src/touch/object_finder/model_finder/model_finder.h
    src/touch/object_finder/object_finder.h
    src/touch/rubber/rubber_ui.h
    src/touch/scrollKinetic/ui_scroll/scroll.h
    src/touch/text/text_widgets/text_widgets.h
    src/touch/text/text_ui/text_ui.h
    src/touch/zoom/zoom_control.h
    src/touch/highlighter/highlighter.h
    src/touch/datastruct/stroke_file.h
    src/touch/datastruct/stroke.h
    src/touch/datastruct/utils_datastruct.h
    src/touch/datastruct/page_index_cache.h
    src/touch/datastruct/datastruct.h
    src/touch/datastruct/page_file.h
    src/touch/datastruct/stroke_drawer.h
    src/touch/datastruct/point.h
    src/touch/datastruct/stroke_complex_data.h
    src/touch/datastruct/page.h
    src/touch/copy_cut/copy_cut_selection.h
    src/touch/pen/pen_ui.h
    src/touch/tabletapplication.h
    src/touch/laser/laser.h
    src/touch/tabletcanvas.h
    src/touch/square/square.h
    src/touch/multi_thread_data.h
    src/touch/event/itspossibletoscroll.h
    src/touch/reduce_size/reduce_size.h
    src/touch/property/property_control.h
    src/audiorecord/audiorecord.h
    src/frompdf/frompdf.h
    src/frompdf/convertImg.h
    src/filelost.h
    src/ui/controlluibutton.h
    src/ui/WStyle.h
    src/ui/uicore.h
    src/ui/toolbar.h
    src/ui/manageMobile.h
    src/windows/showmessageupdate.h
    src/windows/updatecheck.h
    src/windows/mostra_finestra_i.h
    src/currenttitle/merge_currenttitle/merge_currenttitle.h
    src/currenttitle/checksimilecopybook.h
    src/currenttitle/option/modality_data.h
    src/currenttitle/option/option_copybook.h
    src/currenttitle/deleteaudio.h
    src/currenttitle/document.h
    src/images/fromimage.h
    src/last_open/element/element_ui.h
    src/last_open/element/imageWidget/imageWidget.h
    src/last_open/widget_parent/widget_parent.h
    src/last_open/option/option_last_open_ui.h
    src/last_open/struct_last_file.h
    src/last_open/last_open.h
    src/style/inserttable.h
    src/button/button_size.h
    src/sheet/dialog_sheet.h
    src/sheet/style_struct.h
    src/sheet/fast-sheet/fast_sheet_ui.h
    src/core/wline.h
    src/core/core.h
    src/core/wmultiplemutex.h
    src/core/multiarray.h
    src/core/WMutex.h
    src/core/listthreadsave.h
    src/core/wzip.h
    src/testing/testingcore.h
    src/testing/memtest.h
    src/datamanage/retry_save_audio.h
    src/lastedit/redoundo.h
    src/preview/preview.h
    src/videocompress/videocompress_ui.h
    src/videocompress/compressivideo.h
    src/videocompress/enable_videocompress.h
    src/utils/areyousure/areyousure.h
    src/utils/posizione_binario.h
    src/utils/default_location/audio_default_location.h
    src/utils/platform.h
    src/utils/time/waitTime.h
    src/utils/time/current_time.h
    src/utils/changeLanguage.h
    src/utils/removenotwriternote.h
    src/utils/lastModification/lastmodification.h
    src/utils/make_default/make_default_ui.h
    src/utils/permission/permission.h
    src/utils/common_script.h
    src/utils/settings/setting_load.h
    src/utils/common_error_definition.h
    src/utils/get_path_application.h
    src/utils/bug_repo/bug_repo_ui.h
    src/utils/color/color_chooser.h
    src/utils/progress_bar/progress_bar_ui.h
    src/utils/utils.h
    src/utils/color_define_rgb.h
    src/utils/common_def.h
    src/utils/checkpassword/checkpassword.h
    src/utils/aboutQt/aboutqt.h
    src/utils/extract_audio/extract_audio_ui.h
    src/utils/extract_audio/extract_audio.h
    src/utils/get_file_dir/get_file_dir.h
    src/utils/choosepage.h
    src/utils/threadcount.h
    src/utils/remove_key/remove_key.h
    src/utils/slash/slash.h
    src/utils/get_only_name/get_only_name.h
    src/utils/path/get_path.h
    src/utils/path/pathfile.h
    src/utils/retry/retry_ui.h
    src/utils/dialog_critic/dialog_critic.h
    src/utils/mostra_explorer/mostra_explorer.h
    src/utils/copy_text.h
    src/utils/setting_define.h
    src/audiosetting/audioqualityoption.h
    src/audiosetting/savequalita.h
    src/restore_file/get_name_tmp.h
    src/restore_file/ui/setting_restore_ui.h
    src/restore_file/restore_file_critic.h
    src/restore_file/ui_restore/restore.h
    src/audioplay/audioplay.h
    src/audioplay/aggiornotastiriascolto.h

    src/testing/TestingComponent.cpp
    src/core/wzip.h src/core/wzip.cpp
    updater/launch_updater/launch_updater.cpp
    src/testing/memtest.cpp
    src/ui/toolbar.cpp
    src/ui/uicore.cpp
    src/preview_page_widget/list_options.cpp
    src/preview_page_widget/preview_page_container.cpp
    src/preview_page_widget/preview_page_widget.cpp
    src/preview_page_widget/preview_page_item.cpp
    src/core/core.cpp
    src/ui/manageMobile.cpp
    src/touch/object_finder/model/model_ctrl.cpp
    src/touch/datastruct/stroke_drawer.cpp
    src/touch/laser/laser.cpp
    src/touch/datastruct/triggerNewView.cpp
    src/savecopybook.cpp
    src/mainwindow.cpp
    src/dataread/old_version/ver_3/load_file_ver_3.cpp
    src/dataread/old_version/ver_4/load_file_ver_4.cpp
    src/dataread/old_version/ver_7/load_file_ver_7.cpp
    src/dataread/old_version/ver_2/load_file_ver_2.cpp
    src/dataread/old_version/ver_5/load_file_ver_5.cpp
    src/dataread/old_version/ver_6/load_file_ver_6.cpp
    src/dataread/loadfile.cpp
    src/dataread/xmlstruct.cpp
    src/dataread/loadbinario.cpp
    src/dataread/load_from_file.cpp
    src/dataread/readlistarray.cpp
    src/dataread/old_loadbinario/old_version_load_binario.cpp

    src/core/listthreadsave.cpp

    src/cloud/button_mainwindow.cpp

    src/sizeButton.cpp
    src/open_file.cpp
    src/controllUi.cpp
    src/pdf/topdf.cpp
    src/pdf/translate.cpp
    src/openFirstCopybook.cpp
    src/needToSave.cpp
    src/filelost.cpp
    src/dropEvent/dropEvent.cpp
    src/datawrite/renamefile_f_zip.cpp
    src/datawrite/savefile_mainwindow.cpp
    src/datawrite/source_read_ext.cpp
    src/datawrite/write/savefile_savefile.cpp
    src/datawrite/write/salvabinario.cpp
    src/datawrite/write/saveArray.cpp
    src/datawrite/qfilechoose.cpp
    src/main.cpp
    src/log/log_ui/log_ui.cpp
    src/touch/paintevent/loadPixel.cpp
    src/touch/paintevent/paintevent.cpp
    src/touch/rubber/save_setting_rubber_ui.cpp
    src/touch/rubber/update_data.cpp
    src/touch/rubber/load_setting_rubber_ui.cpp
    src/touch/rubber/rubber_ui.cpp
    src/touch/scrollKinetic/scrollKinetic.cpp
    src/touch/scrollKinetic/loadScrollSettings.cpp
    src/touch/scrollKinetic/ui_scroll/scroll.cpp
    src/touch/scrollKinetic/scroll_mainwindow.cpp
    src/touch/text/text_widgets/text_widgets.cpp
    src/touch/text/text_ui/text_ui.cpp
    src/touch/zoom/zoom_control.cpp
    src/touch/zoom/ctrlZoom.cpp
    src/touch/highlighter/highlighter.cpp
    src/touch/datastruct/page.cpp
    src/touch/datastruct/changeId.cpp
    src/touch/datastruct/datastruct.cpp
    src/touch/datastruct/stroke.cpp
    src/touch/datastruct/removePage.cpp
    src/touch/datastruct/controllForRepositioning.cpp
    src/touch/copy_cut/copy_cut_selection.cpp
    src/touch/pen/pen_ui.cpp
    src/touch/square/square.cpp
    src/touch/tabletapplication.cpp
    src/touch/tabletcanvas.cpp
    src/touch/pageCount.cpp
    src/touch/tabletevent.cpp
    src/touch/event/mouseevent.cpp
    src/touch/event/itspossibletoscroll.cpp
    src/touch/event/eventmethod.cpp
    src/touch/event/moving_f.cpp
    src/touch/event/resize_event.cpp
    src/touch/initpixmap.cpp
    src/touch/reduce_size/reduce_size.cpp
    src/touch/reduce_size/button_reduce_size.cpp
    src/touch/property/property_control.cpp
    src/touch/settingmethod.cpp
    src/audiorecord/audiorecord.cpp
    src/audiorecord/start_recording.cpp
    src/frompdf/convertimg.cpp
    src/frompdf/frompdf.cpp
    src/frompdf/metadata.cpp
    src/ui/controlluibutton.cpp
    src/windows/showmessageupdate.cpp
    src/windows/updatecheck.cpp
    src/windows/check_update_mainwindow.cpp
    src/windows/sourcecode_mainwindow.cpp
    src/windows/mostra_finestra_i.cpp
    src/windows/main_updater.cpp
    src/currenttitle/checksimilecopybook.cpp
    src/currenttitle/merge_currenttitle/merge_currenttitle.cpp
    src/currenttitle/deleteaudio.cpp
    src/currenttitle/option/main_exe.cpp
    src/currenttitle/option/option_copybook.cpp
    src/currenttitle/document.cpp
    src/closeevent_file.cpp
    src/images/imagebutton_mainwindow.cpp
    src/images/fromimage.cpp
    src/last_open/element/element_ui.cpp
    src/last_open/element/imageWidget/imageWidget.cpp
    src/last_open/load_data.cpp
    src/last_open/widget_parent/widget_parent.cpp
    src/last_open/last_open.cpp
    src/last_open/option/button_main.cpp
    src/last_open/option/option_last_open_ui.cpp
    src/last_open/save_data.cpp
    src/last_open/recentFileMainWindow.cpp
    src/style/print_action.cpp
    src/button/button_size.cpp
    src/sheet/draw_dialog.cpp
    src/sheet/style_struct.cpp
    src/sheet/fast-sheet/fast_sheet_ui.cpp
    src/sheet/fast_sheet.cpp
    src/sheet/newsheet_mainwindow.cpp
    src/sheet/dialog_sheet.cpp
    src/datamanage/stoprecording.cpp
    src/datamanage/retry_save_audio.cpp
    src/lastedit/redoundo.cpp
    src/lastedit/enableredoundo_mainwindow.cpp
    src/preview/preview.cpp
    src/videocompress/importvideo_functionmainwindow.cpp
    src/videocompress/videocompress_ui.cpp
    src/videocompress/compressivideo.cpp
    src/videocompress/enable_videocompress.cpp
    src/utils/default_location/audio_default_location.cpp
    src/utils/default_location/location_audiorecord.cpp
    src/utils/copy_text.cpp
    src/ui/WStyle.cpp
    src/utils/time/current_time.cpp
    src/utils/threadcount.cpp
    src/utils/changeLanguage.cpp
    src/utils/lastModification/lastmodification.cpp
    src/utils/make_default/make_default_ui.cpp
    src/utils/permission/permission.cpp
    src/utils/settings/setting_load.cpp
    src/utils/removenotwriternote.cpp
    src/utils/bug_repo/bug_repo_ui.cpp
    src/utils/color/color_chooser.cpp
    src/utils/progress_bar/progress_bar_ui.cpp
    src/utils/default_option/default_option_ui.cpp
    src/utils/checkpassword/checkpassword.cpp
    src/utils/aboutQt/aboutqt.cpp
    src/utils/extract_audio/extract_audio.cpp
    src/utils/extract_audio/extract_audio_ui.cpp
    src/utils/extract_audio/extractAudioMain.cpp
    src/utils/get_file_dir/get_file_dir.cpp
    src/utils/remove_key/remove_key.cpp
    src/utils/get_only_name/get_only_name.cpp
    src/utils/path/pathfile.cpp
    src/utils/path/get_path.cpp
    src/utils/retry/retry_ui.cpp
    src/utils/dialog_critic/dialog_critic.cpp
    src/utils/mostra_explorer/mostra_explorer.cpp
    src/audiosetting/savequalita.cpp
    src/audiosetting/audioqualityoption.cpp
    src/restore_file/resFileTmpInTmpFolder.cpp
    src/restore_file/restore_file.cpp
    src/restore_file/ui/setting_restore_ui.cpp
    src/restore_file/restore_file_critic.cpp
    src/restore_file/ui_restore/restore.cpp
    src/audioplay/ctrlAudioPlay.cpp
    src/audioplay/audioplay.cpp
    src/audioplay/aggiornotastiriascolto.cpp
    src/touch/multi_thread_data.cpp
    src/core/wmultiplemutex.cpp
    src/touch/object_finder/object_finder.cpp
    src/touch/object_finder/model/model_line.cpp
    src/touch/object_finder/model/model_circle.cpp
    src/touch/object_finder/model/model_square.cpp
    src/touch/object_finder/model_finder/model_finder.cpp
    src/core/wline.cpp
    src/touch/datastruct/stroke_complex_data.cpp
    src/touch/datastruct/stroke_file.cpp
    src/utils/choosepage.cpp
    src/touch/datastruct/page_file.cpp   
    
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

if(LINUX AND NOT FLATPAK_BUILD AND NOT SNAP)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "~/Qt/6.3.0/gcc_64/lib/cmake")
    message("-- search qt in " ${CMAKE_PREFIX_PATH})
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}Multimedia REQUIRED )
find_package(Qt${QT_VERSION_MAJOR}MultimediaWidgets REQUIRED )
find_package(Qt${QT_VERSION_MAJOR}PrintSupport REQUIRED)

if(NOT APPLE)
    qt_add_executable(${PROJECT_NAME} ${PROJECT_MOC} ${PROJECT_SOURCES})
else()
    qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_MOC} ${PROJECT_SOURCES})
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

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUGINFO")

add_definitions(-DALL_VERSION)

include_directories(src/)

SET (CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -O3")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

if(MACOS_BUILD)
    
endif()

if(FLATPAK_BUILD)
    include_directories(/app/include/)
    include_directories(/app/7app/include/)
    target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libzip.so)
    target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libzip.so.5.4)
    target_link_libraries(${PROJECT_NAME} PRIVATE /app/lib/libzip.so.5)
elseif(NOT FLATPAK_BUILD AND LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib/x86_64-linux-gnu/libzip.so)
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
