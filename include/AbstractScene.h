
#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

// Qt
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLWidget>

class AbstractScene : protected QOpenGLFunctions
{

public:
  AbstractScene(QOpenGLWindow *_window = 0);
  AbstractScene(QOpenGLWidget *_widget = 0);
  virtual ~AbstractScene();
  QOpenGLWindow* window() const;
  QOpenGLWidget* widget() const;
  QOpenGLContext* context();
  const QOpenGLContext* context() const;
  virtual void initialize();
  virtual void paint() = 0;

  void foo();

private:
  QOpenGLWindow *m_window = nullptr;
  QOpenGLWidget *m_widget = nullptr;

};

#endif // ABSTRACTSCENE_H
