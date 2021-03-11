QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    Motor/arduino_control.cpp \
    Motor/motorcontrol.cpp \
    Motor/motorsettingdialog.cpp \
    Motor/smartmotor.cpp \
    Motor/smartmotorporthub.cpp \
    Sqlite/sqliteclass.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Motor/arduino_control.h \
    Motor/motorcontrol.h \
    Motor/motorsettingdialog.h \
    Motor/smartmotor.h \
    Motor/smartmotorporthub.h \
    Sqlite/sqliteclass.h \
    global_define.h \
    mainwindow.h

FORMS += \
    Motor/motorsettingdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/opencv/build/include
DEPENDPATH += $$PWD/opencv/build/x64/vc15/lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/build/x64/vc15/lib/ -lopencv_world401
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/build/x64/vc15/lib/ -lopencv_world401d


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-KMessagePlugin-Desktop_Qt_5_14_2_MSVC2017_64bit-Release/release/ -lKMessagePlugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-KMessagePlugin-Desktop_Qt_5_14_2_MSVC2017_64bit-Release/debug/ -lKMessagePlugin
else:unix: LIBS += -L$$PWD/../build-KMessagePlugin-Desktop_Qt_5_14_2_MSVC2017_64bit-Release/ -lKMessagePlugin

INCLUDEPATH += $$PWD/../build-KMessagePlugin-Desktop_Qt_5_14_2_MSVC2017_64bit-Release/release
DEPENDPATH += $$PWD/../build-KMessagePlugin-Desktop_Qt_5_14_2_MSVC2017_64bit-Release/release
