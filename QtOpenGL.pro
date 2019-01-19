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
    src/GLWidget.cpp \
    src/ControlWidget.cpp \
    src/MainWindow.cpp \
    src/activeobject.cpp \
    src/dynamics/dynamicsWorldController.cpp \
    src/dynamics/abstractconstraint.cpp \
    src/dynamics/collisiondetection.cpp \
    src/dynamics/constraint.cpp \
    src/dynamics/dynamicObject.cpp \
    src/dynamics/dynamicsWorld.cpp \
    src/dynamics/dynamicsWorldController.cpp \
    src/dynamics/particle.cpp \
    src/AbstractScene.cpp \
    src/activeobject.cpp \
    src/camera.cpp \
    src/ControlWidget.cpp \
    src/GLWidget.cpp \
    src/hashgrid.cpp \
    src/inputManager.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    src/model.cpp \
    src/Scene.cpp \
    src/sceneobject.cpp \
    src/shape.cpp \
    src/transform.cpp \
    src/Window.cpp

OBJECTS_DIR = build/obj
MOC_DIR = build/moc

INCLUDEPATH += include

# to include assimp. Not sure if that's the right way
# WIP make it portable
unix: LIBS += -L/usr/local/lib/ -lassimp.3.1.1
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

HEADERS += \
#    include/AbstractScene.h \
#    include/Scene.h \
#    include/Window.h \
#    include/transform.h \
#    include/inputManager.h \
#    include/camera.h \
#    include/sceneobject.h \
#    include/shape.h \
#    include/model.h \
#    include/model.h \
#    include/utils.h \
#    include/dynamicsWorld.h \
#    include/dynamicObject.h
    include/*.h \
    include/dynamics/dynamicObject.h \
    include/dynamics/dynamicsWorld.h \
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
    include/dynamics/particle.h \
    include/dynamics/constraint.h \
    include/dynamics/collisiondetection.h \
    include/dynamics/abstractconstraint.h \
    include/GLWidget.h \
    include/ControlWidget.h \
    include/MainWindow.h \
    include/activeobject.h \
    include/dynamics/dynamicsWorldController.h \
    include/dynamics/abstractconstraint.h \
    include/dynamics/collisiondetection.h \
    include/dynamics/constraint.h \
    include/dynamics/dynamicObject.h \
    include/dynamics/dynamicsWorld.h \
    include/dynamics/dynamicsWorldController.h \
    include/dynamics/particle.h \
    include/AbstractScene.h \
    include/activeobject.h \
    include/camera.h \
    include/ControlWidget.h \
    include/GLWidget.h \
    include/hashgrid.h \
    include/inputManager.h \
    include/MainWindow.h \
    include/model.h \
    include/Scene.h \
    include/sceneobject.h \
    include/shape.h \
    include/transform.h \
    include/utils.h \
    include/Window.h


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







