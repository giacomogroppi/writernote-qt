QT       += core
QT       += gui
QT       += widgets
QT       += multimedia
!ios:QT       += printsupport
QT       += network

macx: DEFINES += "MACOS"
android: DEFINES += "ANDROID_WRITERNOTE"
ios: DEFINES += "IOS_WRITERNOTE"

#Use for testing
#DEFINES += "ANDROID"

CONFIG(release, debug|release){
    message("Optimization to O3")
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O3

    QMAKE_CXXFLAGS_RELEASE *= -fno-exceptions
    QMAKE_CXXFLAGS_DEBUG *= -fno-exceptions
}

#QMAKE_CXXFLAGS_RELEASE += -fanalyzer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = build
RESOURCES     = writernote.qrc \
    language.qrc
CONFIG += c++17

!android{
    #for glibconfig.h
    INCLUDEPATH += /usr/include/glib-2.0/ /usr/lib/x86_64-linux-gnu/glib-2.0/include/
}

INCLUDEPATH += src/

TRANSLATIONS +=  language/it.ts

CONFIG(debug, debug|release){
    DEFINES += "DEBUGINFO"
}

## Snapcraft support
contains(DEFINES, SNAP){
    message("Snapcraft enable")
}

## Support for version
contains(DEFINES, VERSION_SNAPCRAFT){
    message("Version for snapcraft add. Version: $$VERSION_SNAPCRAFT")
}

## for enable cloud instance [remove when compiling for release] [see https://github.com/giacomogroppi/writernote-qt/blob/master/help/compile-cloud.md]

#DEFINES += "CLOUD"

contains(DEFINES, CLOUD){
    message("Testing enable")
    DEFINES += "CLOUD_TESTING"
}

# this macro is used to enable all compatibility with the file
# if you want to build without pass to qmake NO_ALL_VERSION in DEFINES
DEFINES += "ALL_VERSION"
contains(DEFINES, NO_ALL_VERSION){
    DEFINES -= "ALL_VERSION"
    message(Disable all version)
}
## for support A4 pdf create
#DEFINES += "NUMEROPIXELVERTICALI=1400"
#DEFINES += "NUMEROPIXELORIZZONALI=980"

!contains(DEFINES, VERSION_SNAPCRAFT){
    message("Enable testing for version $$VERSION_SNAPCRAFT")
}

QMAKE_CXXFLAGS += -Wno-deprecated-declarations
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/audioplay/audioplay.cpp \
    src/audioplay/ctrlAudioPlay.cpp \
    src/audiorecord/audiorecord.cpp \
    src/audiorecord/start_recording.cpp \
    src/button/button_size.cpp \
    src/controllUi.cpp \
    src/core/listthreadsave.cpp \
    src/currenttitle/default_type/default_type.cpp \
    src/currenttitle/default_type/default_type_button.cpp \
    src/currenttitle/document.cpp \
    src/currenttitle/merge_currenttitle/merge_currenttitle.cpp \
    src/currenttitle/option/main_exe.cpp \
    src/currenttitle/option/option_copybook.cpp \
    src/datamanage/retry_save_audio.cpp \
    src/datamanage/stoprecording.cpp \
    src/dataread/load_from_file.cpp \
    src/dataread/loadfile.cpp \
    src/dataread/old_loadbinario/old_version_load_binario.cpp \
    src/dataread/old_version/ver_2/load_file_ver_2.cpp \
    src/dataread/old_version/ver_3/load_file_ver_3.cpp \
    src/dataread/old_version/ver_4/load_file_ver_4.cpp \
    src/dataread/old_version/ver_5/load_file_ver_5.cpp \
    src/dataread/old_version/ver_6/load_file_ver_6.cpp \
    src/dataread/old_version/ver_7/load_file_ver_7.cpp \
    src/dataread/readlistarray.cpp \
    src/datawrite/write/saveArray.cpp \
    src/dropEvent/dropEvent.cpp \
    src/filelost.cpp \
    src/images/fromimage.cpp \
    src/last_open/option/button_main.cpp \
    src/last_open/option/option_last_open_ui.cpp \
    src/last_open/recentFileMainWindow.cpp \
    src/last_open/widget_parent/widget_parent.cpp \
    src/log/log_ui/log_ui.cpp \
    src/needToSave.cpp \
    src/openFirstCopybook.cpp \
    src/open_file.cpp \
    src/preview/preview.cpp \
    src/restore_file/resFileTmpInTmpFolder.cpp \
    src/restore_file/restore_file.cpp \
    src/restore_file/restore_file_critic.cpp \
    src/restore_file/ui/setting_restore_ui.cpp \
    src/restore_file/ui_restore/restore.cpp \
    src/sheet/fast-sheet/fast_sheet_ui.cpp \
    src/sheet/fast_sheet.cpp \
    src/sizeButton.cpp \
    src/touch/copy_cut/copy_cut_selection.cpp \
    src/touch/datastruct/adjustSize.cpp \
    src/touch/datastruct/changeId.cpp \
    src/touch/datastruct/controllForRepositioning.cpp \
    src/touch/datastruct/createControllPoint.cpp \
    src/touch/datastruct/isinside.cpp \
    src/touch/datastruct/moveAll.cpp \
    src/touch/datastruct/movePoint.cpp \
    src/touch/datastruct/page.cpp \
    src/touch/datastruct/removeIdPoint.cpp \
    src/touch/datastruct/removePage.cpp \
    src/touch/datastruct/reorganize.cpp \
    src/touch/datastruct/stroke.cpp \
    src/touch/datastruct/writtenSomething.cpp \
    src/touch/highlighter/highlighter.cpp \
    src/touch/pageCount.cpp \
    src/touch/paintevent/loadPixel.cpp \
    src/touch/property/property_control.cpp \
    src/touch/reduce_size/button_reduce_size.cpp \
    src/touch/reduce_size/reduce_size.cpp \
    src/touch/scrollKinetic/loadScrollSettings.cpp \
    src/touch/scrollKinetic/scrollKinetic.cpp \
    src/touch/scrollKinetic/scroll_mainwindow.cpp \
    src/touch/scrollKinetic/ui_scroll/scroll.cpp \
    src/touch/text/text_ui/text_ui.cpp \
    src/touch/text/text_widgets/text_widgets.cpp \
    src/touch/zoom/ctrlZoom.cpp \
    src/ui/buttonTablet.cpp \
    src/ui/controlluibutton.cpp \
    src/utils/aboutQt/aboutqt.cpp \
    src/audioplay/aggiornotastiriascolto.cpp \
    src/audiosetting/audioqualityoption.cpp \
    src/audiosetting/savequalita.cpp \
    src/last_open/element/element_ui.cpp \
    src/last_open/last_open.cpp \
    src/last_open/load_data.cpp \
    src/last_open/save_data.cpp \
    src/lastedit/enableredoundo_mainwindow.cpp \
    src/lastedit/redoundo.cpp \
    src/closeevent_file.cpp \
    src/currenttitle/checksimilecopybook.cpp \
    src/currenttitle/deleteaudio.cpp \
    src/dataread/loadbinario.cpp \
    src/datawrite/qfilechoose.cpp \
    src/datawrite/renamefile_f_zip.cpp \
    src/datawrite/write/salvabinario.cpp \
    src/datawrite/savefile_mainwindow.cpp \
    src/datawrite/source_read_ext.cpp \
    src/datawrite/write/savefile_savefile.cpp \
    src/utils/bug_repo/bug_repo_ui.cpp \
    src/utils/changeLanguage.cpp \
    src/utils/checkpassword/checkpassword.cpp \
    src/utils/copy_text.cpp \
    src/utils/default_location/audio_default_location.cpp \
    src/utils/default_location/location_audiorecord.cpp \
    src/utils/default_option/default_option_ui.cpp \
    src/utils/dialog_critic/dialog_critic.cpp \
    src/images/imagebutton_mainwindow.cpp \
    src/indice_class.cpp \
    src/main.cpp \
    src/dataread/xmlstruct.cpp \
    src/mainwindow.cpp \
    src/pdf/topdf.cpp \
    src/pdf/translate.cpp \
    src/savecopybook.cpp \
    src/sheet/dialog_sheet.cpp \
    src/sheet/draw_dialog.cpp \
    src/sheet/newsheet_mainwindow.cpp \
    src/style/print_action.cpp \
    src/touch/datastruct/datastruct.cpp \
    src/touch/datastruct/needtocreatenew.cpp \
    src/touch/event/eventmethod.cpp \
    src/touch/event/itspossibletoscroll.cpp \
    src/touch/event/mouseevent.cpp \
    src/touch/event/moving_f.cpp \
    src/touch/event/resize_event.cpp \
    src/touch/initpixmap.cpp \
    src/touch/paintevent/paintevent.cpp \
    src/touch/pen/pen_ui.cpp \
    src/touch/rubber/load_setting_rubber_ui.cpp \
    src/touch/rubber/rubber_ui.cpp \
    src/touch/rubber/save_setting_rubber_ui.cpp \
    src/touch/rubber/update_data.cpp \
    src/touch/settingmethod.cpp \
    src/touch/square/square.cpp \
    src/touch/tabletapplication.cpp \
    src/touch/tabletcanvas.cpp \
    src/touch/tabletevent.cpp \
    src/touch/zoom/zoom_control.cpp \
    src/utils/color/color_chooser.cpp \
    src/utils/extract_audio/extractAudioMain.cpp \
    src/utils/extract_audio/extract_audio.cpp \
    src/utils/extract_audio/extract_audio_ui.cpp \
    src/utils/get_file_dir/get_file_dir.cpp \
    src/utils/get_only_name/get_only_name.cpp \
    src/utils/lastModification/lastmodification.cpp \
    src/utils/make_default/make_default_ui.cpp \
    src/utils/mostra_explorer/mostra_explorer.cpp \
    src/utils/path/get_path.cpp \
    src/utils/path/pathfile.cpp \
    src/utils/permission/permission.cpp \
    src/utils/progress_bar/progress_bar_ui.cpp \
    src/utils/remove_key/remove_key.cpp \
    src/utils/removenotwriternote.cpp \
    src/utils/retry/retry_ui.cpp \
    src/utils/settings/setting_load.cpp \
    src/utils/threadcount.cpp \
    src/utils/time/current_time.cpp \
    src/videocompress/compressivideo.cpp \
    src/videocompress/enable_videocompress.cpp \
    src/videocompress/importvideo_functionmainwindow.cpp \
    src/videocompress/videocompress_ui.cpp \
    src/windows/check_update_mainwindow.cpp \
    src/windows/main_updater.cpp \
    src/windows/mostra_finestra_i.cpp \
    src/windows/showmessageupdate.cpp \
    src/windows/sourcecode_mainwindow.cpp \
    src/windows/updatecheck.cpp \
    src/sheet/style_struct.cpp \
    src/cloud/button_mainwindow.cpp \
    updater/launch_updater/launch_updater.cpp \
    src/last_open/element/imageWidget/imageWidget.cpp


HEADERS += \
    src/audioplay/audioplay.h \
    src/audiorecord/audiorecord.h \
    src/button/button_size.h \
    src/core/listthreadsave.h \
    src/datamanage/retry_save_audio.h \
    src/last_open/element/imageWidget/imageWidget.h \
    src/currenttitle/default_type/default_type.h \
    src/currenttitle/document.h \
    src/currenttitle/option/modality_data.h \
    src/currenttitle/merge_currenttitle/merge_currenttitle.h \
    src/currenttitle/option/option_copybook.h \
    src/dataread/load_from_file.h \
    src/dataread/readlistarray.h \
    src/filelost.h \
    src/frompdf/convertImg.h \
    src/images/fromimage.h \
    src/last_open/option/option_last_open_ui.h \
    src/last_open/widget_parent/widget_parent.h \
    src/log/log_ui/log_ui.h \
    src/preview/preview.h \
    src/restore_file/get_name_tmp.h \
    src/restore_file/restore_file_critic.h \
    src/restore_file/ui/setting_restore_ui.h \
    src/restore_file/ui_restore/restore.h \
    src/sheet/fast-sheet/fast_sheet_ui.h \
    src/sheet/style_struct.h \
    src/touch/copy_cut/copy_cut_selection.h \
    src/touch/datastruct/page.h \
    src/touch/datastruct/point.h \
    src/touch/datastruct/stroke.h \
    src/touch/highlighter/highlighter.h \
    src/touch/property/property_control.h \
    src/touch/reduce_size/reduce_size.h \
    src/touch/scrollKinetic/ui_scroll/scroll.h \
    src/touch/text/text_ui/text_ui.h \
    src/touch/text/text_widgets/text_widgets.h \
    src/ui/controlluibutton.h \
    src/utils/aboutQt/aboutqt.h \
    src/utils/areyousure/areyousure.h \
    src/audioplay/aggiornotastiriascolto.h \
    src/audiosetting/audioqualityoption.h \
    src/audiosetting/savequalita.h \
    src/last_open/element/element_ui.h \
    src/last_open/last_open.h \
    src/last_open/struct_last_file.h \
    src/lastedit/redoundo.h \
    src/currenttitle/checksimilecopybook.h \
    src/currenttitle/deleteaudio.h \
    src/datawrite/renamefile_f_zip.h \
    src/datawrite/source_read_ext.h \
    src/utils/bug_repo/bug_repo_ui.h \
    src/utils/changeLanguage.h \
    src/utils/checkpassword/checkpassword.h \
    src/utils/color_define_rgb.h \
    src/utils/common_def.h \
    src/utils/common_error_definition.h \
    src/utils/common_script.h \
    src/utils/copy_text.h \
    src/utils/default_location/audio_default_location.h \
    src/utils/default_option/default_option_ui.h \
    src/utils/dialog_critic/dialog_critic.h \
    src/pdf/topdf.h \
    src/sheet/dialog_sheet.h \
    src/touch/datastruct/datastruct.h \
    src/touch/event/itspossibletoscroll.h \
    src/touch/pen/pen_ui.h \
    src/touch/rubber/rubber_ui.h \
    src/touch/square/square.h \
    src/touch/tabletapplication.h \
    src/touch/tabletcanvas.h \
    src/touch/zoom/zoom_control.h \
    src/datawrite/qfilechoose.h \
    src/datawrite/savefile.h \
    src/indice_class.h \
    src/mainwindow.h \
    src/savecopybook.h \
    src/dataread/xmlstruct.h \
    src/utils/color/color_chooser.h \
    src/utils/extract_audio/extract_audio.h \
    src/utils/extract_audio/extract_audio_ui.h \
    src/utils/get_file_dir/get_file_dir.h \
    src/utils/get_only_name/get_only_name.h \
    src/utils/get_path_application.h \
    src/utils/lastModification/lastmodification.h \
    src/utils/make_default/make_default_ui.h \
    src/utils/mostra_explorer/mostra_explorer.h \
    src/utils/path/get_path.h \
    src/utils/path/pathfile.h \
    src/utils/permission/permission.h \
    src/utils/posizione_binario.h \
    src/utils/progress_bar/progress_bar_ui.h \
    src/utils/remove_key/remove_key.h \
    src/utils/removenotwriternote.h \
    src/utils/retry/retry_ui.h \
    src/utils/setting_define.h \
    src/utils/settings/setting_load.h \
    src/utils/slash/slash.h \
    src/utils/threadcount.h \
    src/utils/time/current_time.h \
    src/videocompress/compressivideo.h \
    src/videocompress/enable_videocompress.h \
    src/videocompress/videocompress_ui.h \
    src/windows/mostra_finestra_i.h \
    src/windows/showmessageupdate.h \
    src/windows/updatecheck.h \
    updater/launch_updater/launch_updater.h

FORMS += \
    src/audiosetting/audioqualityoption.ui \
    src/button/button_size.ui \
    src/currenttitle/option/option_copybook.ui \
    src/last_open/last_open.ui \
    src/last_open/option/option_last_open_ui.ui \
    src/last_open/widget_parent/widget_parent.ui \
    src/log/log_ui/log_ui.ui \
    src/mainwindow.ui \
    src/restore_file/ui/setting_restore_ui.ui \
    src/restore_file/ui_restore/restore.ui \
    src/sheet/dialog_sheet.ui \
    src/sheet/fast-sheet/fast_sheet_ui.ui \
    src/touch/highlighter/highlighter.ui \
    src/touch/pen/pen_ui.ui \
    src/touch/property/property_control.ui \
    src/touch/rubber/rubber_ui.ui \
    src/touch/scrollKinetic/ui_scroll/scroll.ui \
    src/touch/text/text_ui/text_ui.ui \
    src/touch/text/text_widgets/text_widgets.ui \
    src/utils/bug_repo/bug_repo_ui.ui \
    src/utils/default_location/audio_default_location.ui \
    src/utils/default_option/default_option_ui.ui \
    src/utils/extract_audio/extract_audio_ui.ui \
    src/utils/make_default/make_default_ui.ui \
    src/utils/progress_bar/progress_bar_ui.ui \
    src/utils/retry/retry_ui.ui \
    src/videocompress/videocompress_ui.ui \
    src/windows/showmessageupdate.ui \
    src/last_open/element/element_ui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32{
    message("Win 32 compilation")
}

win64{
    message("Win 64 compilation")
}

ios{
    message(Enable ios build)
    INCLUDEPATH += 3rdparty/libzip/include/include
    LIBS += 3rdparty/libzip/lib/libzip.a

    OBJECTIVE_SOURCES += ios/src/iosshareutils.mm \
        ios/src/docviewcontroller.mm

    HEADERS += ios/cpp/ios/iosshareutils.hpp \
        ios/cpp/ios/docviewcontroller.h

    # Note for devices: 1=iPhone, 2=iPad, 1,2=Universal.
    QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1,2
}
else:android{
    OTHER_FILES += android/src/org/writernote/utils/QShareUtils.java \
        android/src/org/writernote/examples/sharex/QShareActivity.java \
        android/src/org/writernote/utils/QSharePathResolver.java

    SOURCES += android/cpp/android/androidshareutils.cpp
    SOURCES += android/cpp/android/shareutils.cpp

    HEADERS += android/cpp/android/androidshareutils.h
    HEADERS += android/cpp/android/shareutils.h

    include(3rdparty/android_openssl/openssl.pri)
    message(Enable android build)
    QT += androidextras

    equals(ANDROID_ABIS,"arm64-v8a"){
        message(Enable arm64-v8a android build for libzip)

        LIBS += $$PWD/3rdparty/libzip/android/build/arm64-v8a/../../install/arm64-v8a/lib/libzip.a
        INCLUDEPATH += $$PWD/3rdparty/libzip/android/build/arm64-v8a/../../install/arm64-v8a/include/
    }
    equals(ANDROID_ABIS,"armeabi-v7a"){
        message(Enable armeabi-v7a android build for libzip)

        LIBS += $$PWD/3rdparty/libzip/android/build/armeabi-v7a/../../install/armeabi-v7a/lib/libzip.a
        INCLUDEPATH += $$PWD/3rdparty/libzip/android/build/armeabi-v7a/../../install/armeabi-v7a/include/
    }

    equals(ANDROID_ABIS, "x86"){
        message(Enable x86 android build for libzip)

        LIBS += $$PWD/3rdparty/libzip/android/build/x86/../../install/x86/lib/libzip.a
        INCLUDEPATH += $$PWD/3rdparty/libzip/android/build/x86/../../install/x86/include/
    }
    equals(ANDROID_ABIS, "x86_64"){
        message(Enable x86_64 android build for libzip)

        LIBS += $$PWD/3rdparty/libzip/android/build/x86_64/../../install/x86_64/lib/libzip.a
        INCLUDEPATH += $$PWD/3rdparty/libzip/android/build/x86_64/../../install/x86_64/include/
    }

    message($$ANDROID_ABIS)

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    DISTFILES += \
        android/AndroidManifest.xml


    INCLUDEPATH += $$PWD/android/libzip-android-1.0.1/jni/

}else{
    CONFIG += link_pkgconfig


    if(win32|win64):CONFIG(debug, debug|release){
        ## if we are build qt for debug in qt creator we need to add libzip.a manualy

        message("Debug windows enable")
        LIBS += C:\msys64\mingw64\lib\libzip.dll.a

        INCLUDEPATH += C:\msys64\mingw64\include

    }
    else:macx{
        message(MacOS build)
        #INCLUDEPATH += /usr/local/opt/libzip/include
        #LIBS += /usr/local/opt/libzip/lib/libzip.5.dylib
        INCLUDEPATH += $$PWD/3rdparty/libzip/distrib/include
        LIBS += $$PWD/3rdparty/libzip/lib/libzip.a
        
    }else{
        #if(contains(DEFINES, SNAP)){
        ## static lib for snapcraft package
        #    message(libzip static build)
        #    INCLUDEPATH += /root/parts/libzip-build/build/3rdparty/libzip/distrib/include
        #    LIBS += /root/parts/libzip-build/build/3rdparty/libzip/distrib/lib/libzip.a
        #    LIBS += /root/parts/zlib-build/build/3rdparty/zlib/lib/libz.a
        #}else{
        #    message(libzip from libzip-dev)
        #    PKGCONFIG += libzip
        #}
        message(libzip from libzip-dev)
        PKGCONFIG += libzip
    }


    contains(DEFINES, CLOUD){

        ## cloud file add
        SOURCES += \
            src/cloud/cloud_controll.cpp \
            src/cloud/cloud_ui.cpp \
            src/cloud/struct_file.cpp \
            src/cloud/struct_user.cpp \
            src/cloud/on_select.cpp \
            src/cloud/utils/downloadfile.cpp \
            src/cloud/utils/toencrypt.cpp

        HEADERS += \
            src/cloud/cloud_controll.h \
            src/cloud/cloud_ui.h \
            src/cloud/struct_file.h \
            src/cloud/struct_user.h \
            src/cloud/url.h \
            src/cloud/error_type.h \
            src/cloud/request_type.h \
            src/cloud/struct_mail.h \
            src/cloud/utils/downloadfile.h \
            src/cloud/utils/shared_definition.h \
            src/cloud/utils/toencrypt.h

        FORMS += \
            src/cloud/cloud_ui.ui

    }
    else{
        message("cloud disable for compilation")
    }
}

#ANDROID_EXTRA_LIBS = 

DEFINES += "PDFSUPPORT"
android: DEFINES -= "PDFSUPPORT"
ios: DEFINES -= "PDFSUPPORT"

contains(DEFINES, PDFSUPPORT){
    SOURCES += \
        src/frompdf/convertimg.cpp \
        src/frompdf/metadata.cpp \
        src/frompdf/frompdf.cpp
    HEADERS += \
        src/frompdf/frompdf.h

    message("Enable pdf support")
}

android:contains(DEFINES, PDFSUPPORT){
    equals(ANDROID_ABIS,arm64-v8a){
        message(Enable arm64-v8a android build for poppler)

        INCLUDEPATH += 3rdparty/poppler/install-arm64-v8a/include/poppler/qt5
        LIBS += 3rdparty/poppler/install-arm64-v8a/lib/libpoppler-qt5.a
        LIBS += 3rdparty/poppler/install-arm64-v8a/lib/libpoppler-cpp.a
        LIBS += 3rdparty/poppler/install-arm64-v8a/lib/libpoppler.a
    }
    equals(ANDROID_ABIS,armeabi-v7a){
        message(Enable armeabi-v7a android build for poppler)

        INCLUDEPATH += 3rdparty/poppler/install-armeabi-v7a/include/poppler/qt5
        LIBS += 3rdparty/poppler/install-armeabi-v7a/lib/libpoppler-qt5.a
        LIBS += 3rdparty/poppler/install-armeabi-v7a/lib/libpoppler.a
        LIBS += 3rdparty/poppler/install-armeabi-v7a/lib/libpoppler-cpp.a
    }
}
else:macx:contains(DEFINES, PDFSUPPORT){
    POPPLER_INCLUDE = /usr/local/opt/poppler/include/poppler/qt5
    POPPLER_INCLUDE_CI = /usr/local/opt/poppler-qt5/include/poppler/qt5
    POPPLER_INCLUDE_FIN = ""

    if(exists($${POPPLER_INCLUDE}/poppler-qt5.h)){
        POPPLER_INCLUDE_FIN = $${POPPLER_INCLUDE}
    }else{
        if(exists($${POPPLER_INCLUDE_CI}/poppler-qt5.h)){
            POPPLER_INCLUDE_FIN = $${POPPLER_INCLUDE_CI}
        }else{
            error("Can't locate poppler-qt5 for build")
        }
    }
    INCLUDEPATH += $${POPPLER_INCLUDE_FIN}
    message(Poppler include dir $${POPPLER_INCLUDE_FIN})

    #INCLUDEPATH += /usr/local/opt/poppler/include/poppler/qt5

    #LIBS += /usr/local/Cellar/poppler-qt5/21.07.0/lib/libpoppler-qt5.dylib
    message(Add lib for poppler)
    INCLUDEPATH += /usr/local/opt/poppler-qt5/include/poppler/qt5
    LIBS += /usr/local/opt/poppler-qt5/lib/libpoppler-qt5.dylib
}

else:unix:contains(DEFINES, PDFSUPPORT){
    message(Unix pdf support enable)
    INCLUDEPATH  += /usr/include/poppler/qt5
    LIBS         += -L/usr/lib -lpoppler-qt5
}

win32:contains(DEFINES, PDFSUPPORT){
    INCLUDEPATH  += C:\msys64\mingw64\include\poppler\qt5
    LIBS         += C:\msys64\mingw64\lib\libpoppler-qt5.dll.a
}

DISTFILES += \
    language/it.ts

ANDROID_EXTRA_LIBS =
