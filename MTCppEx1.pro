QT       += core gui concurrent websockets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CTh1.cpp \
    CThMktUpbit.cpp \
    main.cpp \
    CDlgMain.cpp

HEADERS += \
    CDlgMain.h \
    CTh1.h \
    CThMktUpbit.h \
    Variables.h

FORMS += \
    CDlgMain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
