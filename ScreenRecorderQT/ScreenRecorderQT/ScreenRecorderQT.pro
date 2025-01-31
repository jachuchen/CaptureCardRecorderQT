QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AreaSelector.cpp \
    AreaSelectorButtons.cpp \
    GetAudioDevices.cpp \
    MemoryCheckLinux.cpp \
    PlayWidget_yuy2.cpp \
    ScreenRecorder.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AreaSelector.h \
    AreaSelectorButtons.h \
    GetAudioDevices.h \
    MemoryCheckLinux.h \
    PlayWidget_yuy2.h \
    ScreenRecorder.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

win32:LIBS += -lpthread -lole32 -loleaut32

win32: LIBS += -L$$PWD/../lib/ -lavcodec  -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale

unix:LIBS += -lavformat -lavcodec -lavutil -lavdevice -lm -lswscale -lX11 -lpthread -lswresample -lasound


win32:INCLUDEPATH += $$PWD/../include
win32:DEPENDPATH += $$PWD/../include

target.path=/opt/ScreenRecorderQT
target.files=ScreenRecorderQT
icon.path=/opt/ScreenRecorderQT
icon.files=./icons/icon.svg
launcher.path = /usr/share/applications/ScreenRecorderQT
launcher.files = ../ScreenRecorderQT.desktop

unix:INSTALLS += launcher icon
unix:CONFIG += nostrip
