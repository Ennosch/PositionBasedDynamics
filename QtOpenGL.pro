TARGET = QtOpenGLApp
TEMPLATE = app

QT += core gui

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/AbstractScene.cpp \
    src/Scene.cpp \
    src/Window.cpp \
    src/transform.cpp \
    src/inputManager.cpp \
    src/camera.cpp \
    src/sceneobject.cpp \
    src/shape.cpp \
    src/model.cpp

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

# to include assimp. Not sure if that's the right way
# WIP make it portable
unix: LIBS += -L/usr/local/lib/ -lassimp.3.1.1
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

HEADERS += \
    include/AbstractScene.h \
    include/Scene.h \
    include/Window.h \
    include/transform.h \
    include/inputManager.h \
    include/camera.h \
    include/sceneobject.h \
    include/shape.h \
    include/model.h \
    include/model.h \
    include/utils.h


OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md
# add the glsl shader files
    # shaders/*.glsl \

RESOURCES += \
    resources.qrc








