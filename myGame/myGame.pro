QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = myIcon.ico

SOURCES += \
    fruit.cpp \
    main.cpp \
    mybtn.cpp \
    widget.cpp \
    explain.cpp \
    problem.cpp

HEADERS += \
    fruit.h \
    mybtn.h \
    widget.h \
    explain.h \
    problem.h

FORMS += \
    widget.ui \
    explain.ui \
    problem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    res.qrc
