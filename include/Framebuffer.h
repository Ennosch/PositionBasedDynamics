#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLTexture>
#include <QVector3D>
//#include <QOpenGLFunctions>

class Scene;

class Framebuffer
{
public:
    Framebuffer(Scene *_scene);
    bool init();
    void bind();
    void readPixel(uint _x, uint _y);
    void debug();
    uint fbo();

private:
    uint m_fbo, m_pickingTexture, m_depthTexture;
    int WindowWidth;
    int WindowHeight;
    Scene* m_scene;

//    QOpenGLFramebufferObject* qfbo;
//    QOpenGLTexture* color;
//    QOpenGLTexture* depth;

};

#endif // FRAMEBUFFER_H
