QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../
INCLUDEPATH += ../src

DESTDIR = build

SOURCES += \
    ../src/utils/dialog_critic/dialog_critic.cpp \
    ../src/utils/get_file_dir/get_file_dir.cpp \
    ../src/utils/path/get_path.cpp \
    main.cpp \
    updater.cpp

HEADERS += \
    ../src/utils/areyousure/areyousure.h \
    ../src/utils/dialog_critic/dialog_critic.h \
    ../src/utils/get_file_dir/get_file_dir.h \
    ../src/utils/path/get_path.h \
    updater.h

FORMS += \
    updater.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
