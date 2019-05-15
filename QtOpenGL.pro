TARGET = QtOpenGLApp
TEMPLATE = app

QT += core gui

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/Scene.cpp \
    src/Window.cpp \
    src/transform.cpp \
    src/inputManager.cpp


OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

HEADERS += \
    include/Scene.h \
    include/Window.h \
    include/transform.h \
    include/inputManager.h

OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md
# add the glsl shader files
    # shaders/*.glsl \

RESOURCES += \
    resources.qrc
