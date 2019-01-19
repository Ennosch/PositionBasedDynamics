#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitterHandle>

#include "GLWidget.h"
#include "Scene.h"
#include "ControlWidget.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow();
    MainWindow(GLWidget *_glw);
    ~MainWindow();

//protected:
    void setupUi(GLWidget *_glw);


private:
    QWidget *mainWidget;
    GLWidget *glw =  nullptr;
    ControlWidget *controlWidget;
    QHBoxLayout layout;

};

#endif // MAINWINDOW_H
