QT       += widgets core gui dbus
QTPLUGIN += qtvirtualkeyboardplugin
TRANSLATIONS += QtLanguage_ru.ts QtLanguage_en.ts
CODECFORSRC     = UTF-8
CONFIG += lang-ru
CONFIG += lang-ru_RU
CONFIG += lang-all

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ./LIB/ctbtspin.cpp \
    ./LIB/onboard.cpp \
    cpage1.cpp \
    cpage2.cpp \
    cpage3.cpp \
    cribbonopts.cpp \
    main.cpp \
    dialog.cpp

HEADERS += \
    ./LIB/ctbtspin.h \
    ./LIB/onboard.h \
    cpage1.h \
    cpage2.h \
    cpage3.h \
    cribbonopts.h \
    dialog.h

FORMS += \
    cpage1.ui \
    cpage2.ui \
    cpage3.ui \
    dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    stacked_layout.qrc

INCLUDEPATH += ./LIB
