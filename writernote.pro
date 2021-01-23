QT       += core
QT       += gui
QT       += widgets
QT       += multimedia
QT       += printsupport

win32:{
    QT += network
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = build
RESOURCES     = writernote.qrc
CONFIG += c++11

#for glibconfig.h
INCLUDEPATH += /usr/include/glib-2.0/ /usr/lib/x86_64-linux-gnu/glib-2.0/include/

DEFINES += "STAMPA"

DEFINES += "TESTING"

## for support A4 pdf create
DEFINES += "NUMEROPIXELPAGINA=1400"
DEFINES += "NUMEROPIXELORIZZONALI=980"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/areyousure.cpp \
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
    src/datawrite/compressfile.cpp \
    src/datawrite/load/loadfile_xmlstruct.cpp \
    src/datawrite/load/loadindice_xmlstruct.cpp \
    src/datawrite/loadbinario.cpp \
    src/datawrite/qfilechoose.cpp \
    src/datawrite/renamefile_f_zip.cpp \
    src/datawrite/salvabinario.cpp \
    src/datawrite/savefile.cpp \
    src/datawrite/savefile_mainwindow.cpp \
    src/datawrite/source_read_ext.cpp \
    src/dialog_critic.cpp \
    src/images/imagebutton_mainwindow.cpp \
    src/images/save_images.cpp \
    src/images/update_image_mainwindow.cpp \
    src/indice_class.cpp \
    src/main.cpp \
    src/datawrite/xmlstruct.cpp \
    src/mainwindow.cpp \
    src/newcopybook_.cpp \
    src/pdf/draw.cpp \
    src/pdf/topdf.cpp \
    src/pdf/translate.cpp \
    src/pdf/updatebrush_topdf.cpp \
    src/savecopybook.cpp \
    src/self_class.cpp \
    src/setting_ui.cpp \
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
    src/touch/eventmainwindow.cpp \
    src/touch/initpixmap.cpp \
    src/touch/paintevent/disegnofoglio.cpp \
    src/touch/paintevent/draw_image.cpp \
    src/touch/paintevent/gomma.cpp \
    src/touch/paintevent/paintevent.cpp \
    src/touch/paintevent/zoomin.cpp \
    src/touch/paintevent/zoomon.cpp \
    src/touch/riascolto/riascolto.cpp \
    src/touch/settingmethod.cpp \
    src/touch/square/move.cpp \
    src/touch/square/square.cpp \
    src/touch/tabletapplication.cpp \
    src/touch/tabletcanvas.cpp \
    src/touch/tabletevent.cpp \
    src/touch/updatelist.cpp \
    src/update_list_copybook.cpp \
    src/videocompress/compressivideo.cpp \
    src/videocompress/enable_videocompress.cpp \
    src/videocompress/importvideo_functionmainwindow.cpp \
    src/videocompress/videocompress_ui.cpp \
    src/windows/check_update_mainwindow.cpp \
    src/windows/mostra_finestra_i.cpp \
    src/windows/sourcecode_mainwindow.cpp \
    src/windows/updatecheck.cpp


HEADERS += \
    src/areyousure.h \
    src/audioplay/aggiornotastiriascolto.h \
    src/audioplay/cambioiconariascolto.h \
    src/audiosetting/audioqualityoption.h \
    src/audiosetting/definition.h \
    src/audiosetting/loadqualita.h \
    src/audiosetting/savequalita.h \
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
    src/dialog_critic.h \
    src/images/image_struct.h \
    src/images/save_images.h \
    src/newcopybook_.h \
    src/pdf/topdf.h \
    src/setting_ui.h \
    src/style/abilitazioneinput.h \
    src/touch/datastruct/datastruct.h \
    src/touch/event/itspossibletoscroll.h \
    src/touch/method/methoddefinition.h \
    src/touch/paintevent/draw_image.h \
    src/touch/square/square.h \
    src/touch/tabletapplication.h \
    src//touch/tabletcanvas.h \
    src/update_list_copybook.h \
    src/currenttitle/currenttitle_class.h \
    src/datawrite/qfilechoose.h \
    src/datawrite/savefile.h \
    src/indice_class.h \
    src/mainwindow.h \
    src/savecopybook.h \
    src/self_class.h \
    src/datawrite/xmlstruct.h \
    src/style/inserttable_class.h \
    src/videocompress/compressivideo.h \
    src/videocompress/enable_videocompress.h \
    src/videocompress/videocompress_ui.h \
    src/windows/mostra_finestra_i.h \
    src/windows/updatecheck.h

FORMS += \
    src/audiosetting/audioqualityoption.ui \
    src/mainwindow.ui \
    src/style/inserttable_class.ui \
    src/videocompress/videocompress_ui.ui \

# for the filesystem
LIBS += -lglib-2.0


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += link_pkgconfig
PKGCONFIG += libzip

