
#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

// Qt
#include <QOpenGLFunctions>
#include <QOpenGLWindow>

class AbstractScene : protected QOpenGLFunctions
{

public:
  AbstractScene(QOpenGLWindow *_window = 0);
  virtual ~AbstractScene();
  QOpenGLWindow* window() const;
  QOpenGLContext* context();
  const QOpenGLContext* context() const;
  virtual void initialize();
  virtual void paint() = 0;

  void foo();

private:
  QOpenGLWindow *m_window = nullptr;

};

#endif // ABSTRACTSCENE_H
