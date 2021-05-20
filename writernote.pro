QT       += core
QT       += gui
QT       += widgets
QT       += multimedia
QT       += printsupport
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = build
RESOURCES     = writernote.qrc \
    language.qrc
CONFIG += c++17

!android{
    #for glibconfig.h
    INCLUDEPATH += /usr/include/glib-2.0/ /usr/lib/x86_64-linux-gnu/glib-2.0/include/
}

TRANSLATIONS +=  language/it.ts

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
DEFINES += "ALL_VERSION"

## for support A4 pdf create
DEFINES += "NUMEROPIXELPAGINA=1400"
DEFINES += "NUMEROPIXELORIZZONALI=980"

!contains(DEFINES, VERSION_SNAPCRAFT){
    #DEFINES += "VERSION_SNAPCRAFT=1.3.15"
    message("Enable testing for version $$VERSION_SNAPCRAFT")
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/audiorecord/start_recording.cpp \
    src/currenttitle/default_type/default_type.cpp \
    src/currenttitle/default_type/default_type_button.cpp \
    src/currenttitle/merge_currenttitle/merge_currenttitle.cpp \
    src/datamanage/retry_save_audio.cpp \
    src/dropEvent/dropEvent.cpp \
    src/filelost.cpp \
    src/last_open/option/button_main.cpp \
    src/last_open/option/option_last_open_ui.cpp \
    src/lastedit/copy_lastedit.cpp \
    src/lastedit/lista_copybook.cpp \
    src/log/log_ui/log_ui.cpp \
    src/needToSave.cpp \
    src/open_file.cpp \
    src/restore_file/restore_file.cpp \
    src/restore_file/restore_file_critic.cpp \
    src/restore_file/ui/setting_restore_ui.cpp \
    src/restore_file/ui_restore/ui_restore.cpp \
    src/sheet/fast-sheet/fast_sheet_ui.cpp \
    src/sheet/fast_sheet.cpp \
    src/touch/copy/copy_selection.cpp \
    src/touch/copy/copy_widgets.cpp \
    src/touch/datastruct/adjustSize.cpp \
    src/touch/datastruct/changeId.cpp \
    src/touch/datastruct/controllForRepositioning.cpp \
    src/touch/datastruct/createControllPoint.cpp \
    src/touch/datastruct/isinside.cpp \
    src/touch/datastruct/moveAll.cpp \
    src/touch/datastruct/movePoint.cpp \
    src/touch/datastruct/removeIdPoint.cpp \
    src/touch/datastruct/removePage.cpp \
    src/touch/datastruct/reorganize.cpp \
    src/touch/datastruct/writtenSomething.cpp \
    src/touch/pageCount.cpp \
    src/touch/reduce_size/button_reduce_size.cpp \
    src/touch/reduce_size/reduce_size.cpp \
    src/touch/scrollKinetic/loadScrollSettings.cpp \
    src/touch/scrollKinetic/scrollKinetic.cpp \
    src/touch/scrollKinetic/scroll_mainwindow.cpp \
    src/touch/scrollKinetic/ui_scroll/ui_scroll.cpp \
    src/touch/square/draw_square.cpp \
    src/touch/text/text_ui/text_ui.cpp \
    src/touch/text/text_widgets/text_widgets.cpp \
    src/utils/aboutQt/aboutqt.cpp \
    src/utils/areyousure/areyousure.cpp \
    src/audioplay/aggiornotastiriascolto.cpp \
    src/audioplay/audiovolume.cpp \
    src/audioplay/cambiostatoplayer.cpp \
    src/audioplay/enableriascolto.cpp \
    src/audioplay/playbottonfunction.cpp \
    src/audioplay/riascoltoaudioprogressivo.cpp \
    src/audioplay/slidertimechange.cpp \
    src/audioplay/updatestatusbar_riascolto.cpp \
    src/audioplay/valuechangeslidervolume.cpp \
    src/audiorecord/audio_record.cpp \
    src/audiosetting/audioqualityoption.cpp \
    src/audiorecord/displayErrorMessage.cpp \
    src/audiosetting/loadqualita.cpp \
    src/audiosetting/savequalita.cpp \
    src/autosave/autosave_.cpp \
    src/last_open/element/element_ui.cpp \
    src/last_open/last_open.cpp \
    src/last_open/load_data.cpp \
    src/last_open/save_data.cpp \
    src/lastedit/enableredoundo_mainwindow.cpp \
    src/lastedit/redoundo.cpp \
    src/closeevent_file.cpp \
    src/currenttitle/audiototext.cpp \
    src/currenttitle/checksimilecopybook.cpp \
    src/currenttitle/currenttitle_class.cpp \
    src/currenttitle/deleteaudio.cpp \
    src/currenttitle/deletecopybook.cpp \
    src/currenttitle/newcopybook.cpp \
    src/currenttitle/redolist.cpp \
    src/currenttitle/renamefile.cpp \
    src/currenttitle/rightclicklistcopybook.cpp \
    src/datamanage/check1.cpp \
    src/datamanage/checkinserimento.cpp \
    src/datamanage/eliminazioneNFrasi.cpp \
    src/datamanage/setFile_mainwindow.cpp \
    src/datamanage/spacchettamento.cpp \
    src/datamanage/stoprecording.cpp \
    src/datawrite/colortoint.cpp \
    src/dataread/loadfile_xmlstruct.cpp \
    src/dataread/loadindice_xmlstruct.cpp \
    src/dataread/loadbinario.cpp \
    src/datawrite/qfilechoose.cpp \
    src/datawrite/renamefile_f_zip.cpp \
    src/datawrite/write/salvabinario.cpp \
    src/datawrite/savefile.cpp \
    src/datawrite/savefile_mainwindow.cpp \
    src/datawrite/source_read_ext.cpp \
    src/datawrite/write/savefile_savefile.cpp \
    src/datawrite/write/saveindice_savefile.cpp \
    src/utils/bug_repo/bug_repo_ui.cpp \
    src/utils/changeLanguage.cpp \
    src/utils/checkpassword/checkpassword.cpp \
    src/utils/copy_text.cpp \
    src/utils/default_location/audio_default_location.cpp \
    src/utils/default_location/location_audiorecord.cpp \
    src/utils/default_option/default_option_ui.cpp \
    src/utils/dialog_critic/dialog_critic.cpp \
    src/images/imagebutton_mainwindow.cpp \
    src/images/save_images.cpp \
    src/images/update_image_mainwindow.cpp \
    src/indice_class.cpp \
    src/main.cpp \
    src/dataread/xmlstruct.cpp \
    src/mainwindow.cpp \
    src/newcopybook_.cpp \
    src/pdf/draw.cpp \
    src/pdf/topdf.cpp \
    src/pdf/translate.cpp \
    src/pdf/updatebrush_topdf.cpp \
    src/savecopybook.cpp \
    src/setting_ui.cpp \
    src/sheet/dialog_sheet.cpp \
    src/sheet/draw_dialog.cpp \
    src/sheet/newsheet_mainwindow.cpp \
    src/style/abilitazioneinput.cpp \
    src/style/bulletfile.cpp \
    src/style/insertimage.cpp \
    src/style/inserttable_class.cpp \
    src/style/inserttable_mainwindow.cpp \
    src/audioplay/pausariascolto.cpp \
    src/audioplay/stopriascolto.cpp \
    src/style/bold.cpp \
    src/style/print_action.cpp \
    src/touch/datastruct/datastruct.cpp \
    src/touch/datastruct/loadpixel.cpp \
    src/touch/datastruct/needtocreatenew.cpp \
    src/touch/event/eventmethod.cpp \
    src/touch/event/itspossibletoscroll.cpp \
    src/touch/event/mouseevent.cpp \
    src/touch/event/moving_f.cpp \
    src/touch/event/resize_event.cpp \
    src/touch/initpixmap.cpp \
    src/touch/paintevent/disegnofoglio.cpp \
    src/touch/paintevent/draw_image.cpp \
    src/touch/paintevent/paintevent.cpp \
    src/touch/pen/load_settings_pen_ui.cpp \
    src/touch/pen/pen_ui.cpp \
    src/touch/pen/save_settings_pen_ui.cpp \
    src/touch/riascolto/riascolto.cpp \
    src/touch/rubber/load_setting_rubber_ui.cpp \
    src/touch/rubber/rubber_ui.cpp \
    src/touch/rubber/save_setting_rubber_ui.cpp \
    src/touch/rubber/update_data.cpp \
    src/touch/settingmethod.cpp \
    src/touch/square/move.cpp \
    src/touch/square/square.cpp \
    src/touch/tabletapplication.cpp \
    src/touch/tabletcanvas.cpp \
    src/touch/tabletevent.cpp \
    src/touch/updatelist.cpp \
    src/touch/zoom/zoom_control.cpp \
    src/update_list_copybook.cpp \
    src/utils/color/color_chooser.cpp \
    src/utils/extract_audio/extractAudioMain.cpp \
    src/utils/extract_audio/extract_audio.cpp \
    src/utils/extract_audio/extract_audio_ui.cpp \
    src/utils/get_file_dir/get_file_dir.cpp \
    src/utils/make_default/make_default_ui.cpp \
    src/utils/mostra_explorer/mostra_explorer.cpp \
    src/utils/path/get_path.cpp \
    src/utils/permission/permission.cpp \
    src/utils/progress_bar/progress_bar_ui.cpp \
    src/utils/remove_key/remove_key.cpp \
    src/utils/resize_mainwindow/resize.cpp \
    src/utils/retry/retry_ui.cpp \
    src/utils/settings/setting_load.cpp \
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
    src/sheet/load_last_style.cpp \
    src/cloud/button_mainwindow.cpp \
    updater/launch_updater/launch_updater.cpp


HEADERS += \
    src/currenttitle/default_type/default_type.h \
    src/currenttitle/merge_currenttitle/merge_currenttitle.h \
    src/datamanage/retry_save_audio.h \
    src/filelost.h \
    src/last_open/option/option_last_open_ui.h \
    src/lastedit/lista_copybook.h \
    src/log/log_ui/log_ui.h \
    src/restore_file/get_name_tmp.h \
    src/restore_file/restore_file_critic.h \
    src/restore_file/ui/setting_restore_ui.h \
    src/restore_file/ui_restore/ui_restore.h \
    src/sheet/fast-sheet/fast_sheet_ui.h \
    src/touch/copy/copy_selection.h \
    src/touch/copy/copy_widgets.h \
    src/touch/reduce_size/reduce_size.h \
    src/touch/scrollKinetic/ui_scroll/ui_scroll.h \
    src/touch/text/text_ui/text_ui.h \
    src/touch/text/text_widgets/text_widgets.h \
    src/utils/aboutQt/aboutqt.h \
    src/utils/abs.h \
    src/utils/areyousure/areyousure.h \
    src/audioplay/aggiornotastiriascolto.h \
    src/audioplay/cambioiconariascolto.h \
    src/audiosetting/audioqualityoption.h \
    src/audiosetting/loadqualita.h \
    src/audiosetting/savequalita.h \
    src/autosave/autosave_.h \
    src/last_open/element/element_ui.h \
    src/last_open/last_open.h \
    src/last_open/struct_last_file.h \
    src/lastedit/redoundo.h \
    src/currenttitle/audiototext.h \
    src/currenttitle/checksimilecopybook.h \
    src/currenttitle/deleteaudio.h \
    src/currenttitle/deletecopybook.h \
    src/currenttitle/redolist.h \
    src/currenttitle/renamefile.h \
    src/datamanage/check1.h \
    src/datamanage/checkinserimento.h \
    src/datamanage/spacchettamento.h \
    src/datawrite/colortoint.h \
    src/datawrite/renamefile_f_zip.h \
    src/datawrite/sizedata.h \
    src/datawrite/source_read_ext.h \
    src/utils/bug_repo/bug_repo_ui.h \
    src/utils/changeLanguage.h \
    src/utils/checkpassword/checkpassword.h \
    src/utils/color/setcolor.h \
    src/utils/common_def.h \
    src/utils/common_error_definition.h \
    src/utils/copy_text.h \
    src/utils/default_location/audio_default_location.h \
    src/utils/default_option/default_option_ui.h \
    src/utils/dialog_critic/dialog_critic.h \
    src/images/image_struct.h \
    src/images/save_images.h \
    src/newcopybook_.h \
    src/pdf/topdf.h \
    src/setting_ui.h \
    src/sheet/dialog_sheet.h \
    src/style/abilitazioneinput.h \
    src/touch/datastruct/datastruct.h \
    src/touch/event/itspossibletoscroll.h \
    src/touch/paintevent/draw_image.h \
    src/touch/pen/pen_ui.h \
    src/touch/rubber/rubber_ui.h \
    src/touch/square/square.h \
    src/touch/tabletapplication.h \
    src/touch/tabletcanvas.h \
    src/touch/zoom/zoom_control.h \
    src/currenttitle/currenttitle_class.h \
    src/datawrite/qfilechoose.h \
    src/datawrite/savefile.h \
    src/indice_class.h \
    src/mainwindow.h \
    src/savecopybook.h \
    src/dataread/xmlstruct.h \
    src/style/inserttable_class.h \
    src/utils/color/color_chooser.h \
    src/utils/extract_audio/extract_audio.h \
    src/utils/extract_audio/extract_audio_ui.h \
    src/utils/get_file_dir/get_file_dir.h \
    src/utils/make_default/make_default_ui.h \
    src/utils/mostra_explorer/mostra_explorer.h \
    src/utils/path/get_path.h \
    src/utils/permission/permission.h \
    src/utils/posizione_binario.h \
    src/utils/progress_bar/progress_bar_ui.h \
    src/utils/remove_key/remove_key.h \
    src/utils/retry/retry_ui.h \
    src/utils/setting_define.h \
    src/utils/settings/setting_load.h \
    src/utils/slash/slash.h \
    src/utils/time/current_time.h \
    src/videocompress/compressivideo.h \
    src/videocompress/enable_videocompress.h \
    src/videocompress/videocompress_ui.h \
    src/windows/mostra_finestra_i.h \
    src/windows/showmessageupdate.h \
    src/windows/updatecheck.h \
    src/sheet/load_last_style.h \
    updater/launch_updater/launch_updater.h

FORMS += \
    src/audiosetting/audioqualityoption.ui \
    src/last_open/element/element_ui.ui \
    src/last_open/last_open.ui \
    src/last_open/option/option_last_open_ui.ui \
    src/log/log_ui/log_ui.ui \
    src/mainwindow.ui \
    src/restore_file/ui/setting_restore_ui.ui \
    src/restore_file/ui_restore/ui_restore.ui \
    src/sheet/dialog_sheet.ui \
    src/sheet/fast-sheet/fast_sheet_ui.ui \
    src/style/inserttable_class.ui \
    src/touch/copy/copy_widgets.ui \
    src/touch/pen/pen_ui.ui \
    src/touch/rubber/rubber_ui.ui \
    src/touch/scrollKinetic/ui_scroll/ui_scroll.ui \
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
    src/windows/showmessageupdate.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32{
    message("Win 32 compilation")
}

win64{
    message("Win 64 comilation")
}

android{
    QT += androidextras

    LIBS += $$PWD/android/libzip-android-1.0.1/obj/local/armeabi-v7a/libzip.a
    #LIBS += $$PWD/android/libzip/libzip-android-1.0.1/obj/local/armeabi-v7a/libzip.a

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/res/values/libs.xml


    INCLUDEPATH += $$PWD/android/libzip-android-1.0.1/jni/

}else{
    CONFIG += link_pkgconfig


    if(win32|win64):CONFIG(debug, debug|release){
        ## if we are build qt for debug in qt creator we need to add libzip.a manualy

        message("Debug windows enable")
        LIBS += C:\msys64\mingw64\lib\libzip.dll.a

        INCLUDEPATH += C:\msys64\mingw64\include

    }
    else{
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

        message( "Adding lib for crypto" )
        unix{
            LIBS += -lcryptopp
        }
        else{
            LIBS += crypto++
        }
    }
    else{
        message("cloud disable for compilation")
    }
}


android: include(/home/giacomo/Android/Sdk/android_openssl/openssl.pri)

ANDROID_EXTRA_LIBS = /home/giacomo/Android/Sdk/android_openssl/latest/arm/libcrypto_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/arm/libssl_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/arm64/libcrypto_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/arm64/libssl_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/x86/libcrypto_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/x86/libssl_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/x86_64/libcrypto_1_1.so /home/giacomo/Android/Sdk/android_openssl/latest/x86_64/libssl_1_1.so

unix:contains(DEFINES, PDFSUPPORT){
    message("Add support for pdf")

    LIBS += -L/usr/lib/x86_64-linux-gnu/ -lQt5Pdfium

    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtPdfium
    DEPENDPATH += /usr/include/x86_64-linux-gnu/qt5/QtPdfium

    SOURCES += \
        src/frompdf/frompdf.cpp
    HEADERS += \
        src/frompdf/frompdf.h
}

DISTFILES += \
    language/it.ts
