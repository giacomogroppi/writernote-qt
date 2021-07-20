cmake_minimum_required(VERSION 3.5)

project(writernote LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED )
find_package(Qt5MultimediaWidgets REQUIRED )
find_package(Qt5PrintSupport REQUIRED)

SET(QT_USE_QTMULTIMEDIA TRUE)
SET(QT_USE_QTMULTIMEDIAWIDGETS TRUE)
set(QT_USE_PRINTSUPPORT TRUE)

set(PROJECT_SOURCES
    src/audiorecord/start_recording.cpp 
    src/changeCopybook.cpp 
    src/controllUi.cpp 
    src/currenttitle/default_type/default_type.cpp 
    src/currenttitle/default_type/default_type_button.cpp 
    src/currenttitle/document.cpp 
    src/currenttitle/merge_currenttitle/merge_currenttitle.cpp 
    src/currenttitle/option/main_exe.cpp 
    src/currenttitle/option/option_copybook.cpp 
    src/datamanage/retry_save_audio.cpp 
    src/dataread/load_from_file.cpp 
    src/dropEvent/dropEvent.cpp 
    src/filelost.cpp 
    src/last_open/option/button_main.cpp 
    src/last_open/option/option_last_open_ui.cpp 
    src/lastedit/copy_lastedit.cpp 
    src/lastedit/lista_copybook.cpp 
    src/log/log_ui/log_ui.cpp 
    src/needToSave.cpp 
    src/openFirstCopybook.cpp 
    src/open_file.cpp 
    src/restore_file/get_name_available.cpp 
    src/restore_file/resFileTmpInTmpFolder.cpp 
    src/restore_file/restore_file.cpp 
    src/restore_file/restore_file_critic.cpp 
    src/restore_file/ui/setting_restore_ui.cpp 
    src/restore_file/ui_restore/restore.cpp 
    src/sheet/fast-sheet/fast_sheet_ui.cpp 
    src/sheet/fast_sheet.cpp 
    src/touch/copy/copy_selection.cpp 
    src/touch/copy/copy_widgets.cpp 
    src/touch/datastruct/adjustSize.cpp 
    src/touch/datastruct/changeId.cpp 
    src/touch/datastruct/controllForRepositioning.cpp 
    src/touch/datastruct/createControllPoint.cpp 
    src/touch/datastruct/isinside.cpp 
    src/touch/datastruct/moveAll.cpp 
    src/touch/datastruct/movePoint.cpp 
    src/touch/datastruct/removeIdPoint.cpp 
    src/touch/datastruct/removePage.cpp 
    src/touch/datastruct/reorganize.cpp 
    src/touch/datastruct/writtenSomething.cpp 
    src/touch/highlighter/highlighter.cpp 
    src/touch/pageCount.cpp 
    src/touch/reduce_size/button_reduce_size.cpp 
    src/touch/reduce_size/reduce_size.cpp 
    src/touch/scrollKinetic/loadScrollSettings.cpp 
    src/touch/scrollKinetic/scrollKinetic.cpp 
    src/touch/scrollKinetic/scroll_mainwindow.cpp 
    src/touch/scrollKinetic/ui_scroll/scroll.cpp 
    src/touch/square/draw_square.cpp 
    src/touch/text/text_ui/text_ui.cpp 
    src/touch/text/text_widgets/text_widgets.cpp 
    src/utils/aboutQt/aboutqt.cpp 
    src/audioplay/aggiornotastiriascolto.cpp 
    src/audioplay/audiovolume.cpp 
    src/audioplay/cambiostatoplayer.cpp 
    src/audioplay/enableriascolto.cpp 
    src/audioplay/playbottonfunction.cpp 
    src/audioplay/riascoltoaudioprogressivo.cpp 
    src/audioplay/slidertimechange.cpp 
    src/audioplay/updatestatusbar_riascolto.cpp 
    src/audioplay/valuechangeslidervolume.cpp 
    src/audiosetting/audioqualityoption.cpp 
    src/audiorecord/displayErrorMessage.cpp 
    src/audiosetting/loadqualita.cpp 
    src/audiosetting/savequalita.cpp 
    src/autosave/autosave_.cpp 
    src/last_open/element/element_ui.cpp 
    src/last_open/last_open.cpp 
    src/last_open/load_data.cpp 
    src/last_open/save_data.cpp 
    src/lastedit/enableredoundo_mainwindow.cpp 
    src/lastedit/redoundo.cpp 
    src/closeevent_file.cpp 
    src/currenttitle/audiototext.cpp 
    src/currenttitle/checksimilecopybook.cpp 
    src/currenttitle/deleteaudio.cpp 
    src/currenttitle/deletecopybook.cpp 
    src/currenttitle/newcopybook.cpp 
    src/currenttitle/redolist.cpp 
    src/currenttitle/renamefile.cpp 
    src/currenttitle/rightclicklistcopybook.cpp 
    src/datamanage/check1.cpp 
    src/datamanage/checkinserimento.cpp 
    src/datamanage/eliminazioneNFrasi.cpp 
    src/datamanage/spacchettamento.cpp 
    src/datamanage/stoprecording.cpp 
    src/dataread/loadfile_xmlstruct.cpp 
    src/dataread/loadindice_xmlstruct.cpp 
    src/dataread/loadbinario.cpp 
    src/datawrite/qfilechoose.cpp 
    src/datawrite/renamefile_f_zip.cpp 
    src/datawrite/write/salvabinario.cpp 
    src/datawrite/savefile_mainwindow.cpp 
    src/datawrite/source_read_ext.cpp 
    src/datawrite/write/savefile_savefile.cpp 
    src/datawrite/write/saveindice_savefile.cpp 
    src/utils/bug_repo/bug_repo_ui.cpp 
    src/utils/changeLanguage.cpp 
    src/utils/checkpassword/checkpassword.cpp 
    src/utils/copy_text.cpp 
    src/utils/default_location/audio_default_location.cpp 
    src/utils/default_location/location_audiorecord.cpp 
    src/utils/default_option/default_option_ui.cpp 
    src/utils/dialog_critic/dialog_critic.cpp 
    src/images/imagebutton_mainwindow.cpp 
    src/images/save_images.cpp 
    src/images/update_image_mainwindow.cpp 
    src/indice_class.cpp 
    src/main.cpp 
    src/dataread/xmlstruct.cpp 
    src/mainwindow.cpp 
    src/newcopybook_.cpp 
    src/pdf/draw.cpp 
    src/pdf/topdf.cpp 
    src/pdf/translate.cpp 
    src/pdf/updatebrush_topdf.cpp 
    src/savecopybook.cpp 
    src/setting_ui.cpp 
    src/sheet/dialog_sheet.cpp 
    src/sheet/draw_dialog.cpp 
    src/sheet/newsheet_mainwindow.cpp 
    src/style/bulletfile.cpp 
    src/style/insertimage.cpp 
    src/style/inserttable_class.cpp 
    src/style/inserttable_mainwindow.cpp 
    src/audioplay/stopriascolto.cpp 
    src/style/bold.cpp 
    src/style/print_action.cpp 
    src/touch/datastruct/datastruct.cpp 
    src/touch/datastruct/loadpixel.cpp 
    src/touch/datastruct/needtocreatenew.cpp 
    src/touch/event/eventmethod.cpp 
    src/touch/event/itspossibletoscroll.cpp 
    src/touch/event/mouseevent.cpp 
    src/touch/event/moving_f.cpp 
    src/touch/event/resize_event.cpp 
    src/touch/initpixmap.cpp 
    src/touch/paintevent/disegnofoglio.cpp 
    src/touch/paintevent/draw_image.cpp 
    src/touch/paintevent/paintevent.cpp 
    src/touch/pen/load_settings_pen_ui.cpp 
    src/touch/pen/pen_ui.cpp 
    src/touch/pen/save_settings_pen_ui.cpp 
    src/touch/riascolto/riascolto.cpp 
    src/touch/rubber/load_setting_rubber_ui.cpp 
    src/touch/rubber/rubber_ui.cpp 
    src/touch/rubber/save_setting_rubber_ui.cpp 
    src/touch/rubber/update_data.cpp 
    src/touch/settingmethod.cpp 
    src/touch/square/move.cpp 
    src/touch/square/square.cpp 
    src/touch/tabletapplication.cpp 
    src/touch/tabletcanvas.cpp 
    src/touch/tabletevent.cpp 
    src/touch/updatelist.cpp 
    src/touch/zoom/zoom_control.cpp 
    src/update_list_copybook.cpp 
    src/utils/color/color_chooser.cpp 
    src/utils/extract_audio/extractAudioMain.cpp 
    src/utils/extract_audio/extract_audio.cpp 
    src/utils/extract_audio/extract_audio_ui.cpp 
    src/utils/get_file_dir/get_file_dir.cpp 
    src/utils/make_default/make_default_ui.cpp 
    src/utils/mostra_explorer/mostra_explorer.cpp 
    src/utils/path/get_path.cpp 
    src/utils/permission/permission.cpp 
    src/utils/progress_bar/progress_bar_ui.cpp 
    src/utils/remove_key/remove_key.cpp 
    src/utils/resize_mainwindow/resize.cpp 
    src/utils/retry/retry_ui.cpp 
    src/utils/settings/setting_load.cpp 
    src/utils/time/current_time.cpp 
    src/videocompress/compressivideo.cpp 
    src/videocompress/enable_videocompress.cpp 
    src/videocompress/importvideo_functionmainwindow.cpp 
    src/videocompress/videocompress_ui.cpp 
    src/windows/check_update_mainwindow.cpp 
    src/windows/main_updater.cpp 
    src/windows/mostra_finestra_i.cpp 
    src/windows/showmessageupdate.cpp 
    src/windows/sourcecode_mainwindow.cpp 
    src/windows/updatecheck.cpp 
    src/sheet/load_last_style.cpp 
    src/cloud/button_mainwindow.cpp 
    updater/launch_updater/launch_updater.cpp

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
    src/style/inserttable_class.ui 
    src/touch/copy/copy_widgets.ui 
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

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(writernote
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(writernote SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(writernote
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	AUTOUIC TRUE
	AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR};${PROJECT_SOURCE_DIR}/src"
	AUTOMOC TRUE
	AUTORCC TRUE
	AUTOGEN_SOURCE_GROUP "Qt Files"
)

add_definitions(-DALL_VERSION)
add_definitions(-DNUMEROPIXELORIZZONALI=980)
add_definitions(-DNUMEROPIXELPAGINA=1400)
add_definitions(-DPDF_SUPPORT)

target_include_directories(${PROJECT_NAME} PRIVATE /usr/include/poppler/qt5) 

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::PrintSupport)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib/x86_64-linux-gnu/libzip.so)
target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib/x86_64-linux-gnu/libpoppler-qt5.so)