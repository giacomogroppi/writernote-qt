QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = build

SOURCES += \
    ../src/utils/dialog_critic/dialog_critic.cpp \
    ../src/utils/path/get_path.cpp \
    main.cpp \
    updater.cpp

HEADERS += \
    ../src/utils/areyousure/areyousure.h \
    ../src/utils/dialog_critic/dialog_critic.h \
    ../src/utils/path/get_path.h \
    updater.h

FORMS += \
    updater.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
    PATH_CI = C:Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64\bin\x64\mt.exe
    PATH = C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x64\mt.exe
    if(exists( $$PATH_CI )){
         WINSDK_DIR = $$PATH_CI
    }else{
        WINSDK_DIR = $$PATH
    }

    WIN_PWD = $$replace(PWD, /, \\)
    OUT_PWD_WIN = $$replace(OUT_PWD, /, \\)

    TO = "$$OUT_PWD_WIN\\build\\updater.exe"
    QMAKE_POST_LINK = "'$$WINSDK_DIR' -manifest '$$quote($$WIN_PWD\\$$basename(TARGET).exe.manifest)' -outputresource:$$TO;1"
}
#RC_FILE = res_manifest.rc
