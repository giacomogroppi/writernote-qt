QT       += core gui
QT       += core
QT       += widgets
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = build
RESOURCES     = writernote.qrc
CONFIG += c++11

#for glibconfig.h
INCLUDEPATH += /usr/include/glib-2.0/ /usr/lib/x86_64-linux-gnu/glib-2.0/include/

DEFINES += "STAMPA"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/audioplay/aggiornotastiriascolto.cpp \
    src/audioplay/cambiostatoplayer.cpp \
    src/audioplay/enableriascolto.cpp \
    src/audioplay/playbottonfunction.cpp \
    src/audioplay/riascoltoaudioprogressivo.cpp \
    src/audiosetting/audioqualityoption.cpp \
    src/audiorecord/displayErrorMessage.cpp \
    src/audiorecord/getbufferlevels.cpp \
    src/audiosetting/loadqualita.cpp \
    src/audiosetting/savequalita.cpp \
    src/chartoint.cpp \
    src/currenttitle/audiototext.cpp \
    src/currenttitle/compressvideo.cpp \
    src/currenttitle/currenttitle_class.cpp \
    src/currenttitle/deleteaudio.cpp \
    src/currenttitle/deletecopybook.cpp \
    src/currenttitle/redolist.cpp \
    src/currenttitle/renamefile.cpp \
    src/currenttitle/rightclicklistcopybook.cpp \
    src/datamanage/check1.cpp \
    src/datamanage/checkinserimento.cpp \
    src/datamanage/eliminazioneNFrasi.cpp \
    src/datamanage/sistemazionedatiregistrati.cpp \
    src/datamanage/spacchettamento.cpp \
    src/datamanage/stoprecording.cpp \
    src/datawrite/qfilechoose.cpp \
    src/datawrite/renamefile_f_zip.cpp \
    src/datawrite/savefile.cpp \
    src/dialog_critic.cpp \
    src/indice_class.cpp \
    src/main.cpp \
    src/datawrite/xmlstruct.cpp \
    src/mainwindow.cpp \
    src/newcopybook_.cpp \
    src/savecopybook.cpp \
    src/self_class.cpp \
    src/setting_ui.cpp \
    src/stringgenerator.cpp \
    src/audiorecord/audiolevel.cpp \
    src/style/bulletfile.cpp \
    src/style/insertimage.cpp \
    src/style/inserttable.cpp \
    src/style/inserttable_class.cpp \
    src/style/main_style.cpp \
    src/audioplay/pausariascolto.cpp \
    src/audioplay/stopriascolto.cpp \
    src/audioplay/aggiornamentostatusbar.cpp \
    src/style/bold.cpp \
    src/audioplay/main_audioplay.cpp


HEADERS += \
    src/areyousure.h \
    src/audiosetting/audioqualityoption.h \
    src/audiorecord/getbufferlevels.h \
    src/audiosetting/definition.h \
    src/audiosetting/loadqualita.h \
    src/audiosetting/savequalita.h \
    src/currenttitle/main_include_currenttitle.h \
    src/currenttitle/redolist.h \
    src/datawrite/renamefile_f_zip.h \
    src/dialog_critic.h \
    src/chartoint.h \
    src/newcopybook_.h \
    src/setting_ui.h \
    src/update_list_copybook.h \
    src/currenttitle/currenttitle_class.h \
    src/datawrite/qfilechoose.h \
    src/datawrite/savefile.h \
    src/indice_class.h \
    src/mainwindow.h \
    src/savecopybook.h \
    src/self_class.h \
    src/audiorecord/audiolevel.h\
    src/datawrite/xmlstruct.h \
    src/style/inserttable_class.h

FORMS += \
    src/audiosetting/audioqualityoption.ui \
    src/mainwindow.ui \
    src/style/inserttable_class.ui

# for the filesystem
LIBS += -lstdc++fs -lglib-2.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libzip
