TARGET = QtOpenGLApp
TEMPLATE = app

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    src/model.cpp \
    src/hashgrid.cpp \
    src/dynamics/dynamicObject.cpp \
    src/dynamics/dynamicsWorld.cpp \
    src/dynamics/particle.cpp \
    src/dynamics/constraint.cpp \
    src/dynamics/collisiondetection.cpp \
    src/dynamics/abstractconstraint.cpp \
    src/dynamics/dynamicsWorldController.cpp \
    src/GLWidget.cpp \
    src/ControlWidget.cpp \
    src/MainWindow.cpp \
    src/activeobject.cpp \
    src/ui/DynamicsUiWidget.cpp \
    src/manipulator.cpp \
    src/Framebuffer.cpp \
    src/dynamics/rigidBody.cpp \
    src/dynamics/softBody.cpp


OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

# to include assimp. Not sure if that's the right way
# WIP make it portable
unix: LIBS += -L/usr/local/lib/ -lassimp.3.1.1
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

HEADERS += \
    include/*.h \
    include/AbstractScene.h \
    include/camera.h \
    include/inputManager.h \
    include/model.h \
    include/Scene.h \
    include/sceneobject.h \
    include/shape.h \
    include/transform.h \
    include/utils.h \
    include/Window.h \
    include/hashgrid.h \
    include/dynamics/dynamicsWorldController.h \
    include/dynamics/dynamicObject.h \
    include/dynamics/dynamicsWorld.h \
    include/dynamics/particle.h \
    include/dynamics/constraint.h \
    include/dynamics/collisiondetection.h \
    include/dynamics/abstractconstraint.h \
    include/GLWidget.h \
    include/ControlWidget.h \
    include/MainWindow.h \
    include/activeobject.h \
    include/ui/DynamicsUiWidget.h \
    include/manipulator.h \
    include/Framebuffer.h \
    include/dynamics/rigidBody.h \
    include/dynamics/dynamicUtils.h \
    include/dynamics/softBody.h



OTHER_FILES += \
    .gitignore \
    Doxyfile \
    README.md
# add the glsl shader files
    # shaders/*.glsl \

RESOURCES += \
    resources.qrc

FORMS += \
    resources/Forms/ControlWidget.ui







