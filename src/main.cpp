#include <iostream>

#include <QGuiApplication>
#include <QOpenGLFunctions>
#include "Window.h"
#include "Scene.h"



int main(int argc, char *argv[])
{

//    Transform x;

//    uint test;
//    glGenBuffers(100, &test);

    QGuiApplication app(argc, argv);

    Window window;



    QSurfaceFormat fmt;
    fmt.setMinorVersion(3);
    fmt.setMajorVersion(3);
    fmt.setSamples(16);
    fmt.setSwapInterval(1);


    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    window.setFormat(fmt);


    Scene scene(&window);
    window.setScene(&scene);

    window.resize(720, 720);
    window.show();

    std::cout<<"---k"<<std::endl;

    return app.exec();
}
