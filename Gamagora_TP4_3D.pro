#-------------------------------------------------
#
# Project created by QtCreator 2015-12-02T00:35:20
#
#-------------------------------------------------

QT       += core
QT       += gui

TARGET = Gamagora_TP4_3D
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lopengl32 -lglu32

INCLUDEPATH += C:\lib
INCLUDEPATH += C:\Developpement\glm
INCLUDEPATH += C:\librairie\glm


QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp


CONFIG  += c++11

QT += opengl



SOURCES += main.cpp \
    node.cpp \
    export/objmanager.cpp \
    lib/box.cpp \
    lib/interpolation.cpp \
    lib/noisegenerator.cpp \
    lib/nrw.cpp \
    lib/sphere.cpp \
    lib/voxel.cpp \
    rendu/camera.cpp \
    rendu/colorgradient.cpp \
    rendu/myglwidget.cpp \
    rendu/mywindow.cpp \
    rendu/rayon.cpp \
    rendu/scene.cpp \
    terrain/terrain.cpp \
    terrain/terrainnoise.cpp \
    terrain/terraintab.cpp \
    texturing/texture.cpp \
    texturing/lumiere.cpp

HEADERS += \
    node.h \
    export/objmanager.h \
    lib/box.h \
    lib/interpolation.h \
    lib/noisegenerator.h \
    lib/nrw.h \
    lib/parametres.h \
    lib/sphere.h \
    lib/voxel.h \
    rendu/camera.h \
    rendu/colorgradient.h \
    rendu/myglwidget.h \
    rendu/mywindow.h \
    rendu/rayon.h \
    rendu/scene.h \
    terrain/terrain.h \
    terrain/terrainnoise.h \
    terrain/terraintab.h \
    texturing/texture.h \
    texturing/lumiere.h

RESOURCES += \
    myresources.qrc
