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
    src/shape.cpp


OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

HEADERS += \
    include/AbstractScene.h \
    include/Scene.h \
    include/Window.h \
    include/transform.h \
    include/inputManager.h \
    include/camera.h \
    include/sceneobject.h \
    include/shape.h

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md
# add the glsl shader files
    # shaders/*.glsl \

RESOURCES += \
    resources.qrc
