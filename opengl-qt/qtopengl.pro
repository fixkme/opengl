#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T16:41:15
#
#-------------------------------------------------

QT       += core gui \
            core    \
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtopengl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    camera.cpp \
    light.cpp \
    skybox.cpp \
    mesh.cpp \
    model.cpp \
    gameobject.cpp \
    infowidget.cpp \
    objwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    camera.h \
    light.h \
    skybox.h \
    mesh.h \
    stb_image.h \
    model.h \
    gameobject.h \
    infowidget.h \
    objwidget.h


FORMS    += mainwindow.ui \
    infowidget.ui \
    objwidget.ui

RESOURCES += \
    res.qrc


INCLUDEPATH += $$PWD/include

LIBS += $$PWD/lib/*.lib

