#include "include/Framebuffer.h"

#include "Scene.h"

#include <QDebug>
#include <iostream>




Framebuffer::Framebuffer()
{
    WindowWidth = 1392;
    WindowHeight = 1452;
    ms = true;
    init();
}

Framebuffer::Framebuffer(Scene *_scene) :
    m_scene(_scene)
{
    WindowWidth = 1392;
    WindowHeight = 1452;

    ms = true;
    init();
}

bool Framebuffer::init()
{
    qDebug()<<"FRAMEBUFFER::INIT"<<WindowWidth<<WindowHeight;
//    glGenFramebuffers(1, &m_msfbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);

    //  Create the texture object for the primitive information buffer
        glGenTextures(1, &m_msColorBuffer);
        if(ms){ glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msColorBuffer);}
        else{glBindTexture(GL_TEXTURE_2D, m_msColorBuffer);}

        if(ms){ glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, WindowWidth, WindowHeight, GL_TRUE);}
        else{glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);}

        if(ms){ glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_msColorBuffer, 0);}
        else{glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_msColorBuffer, 0);}


    //  Create the texture object for the depth buffer
        glGenTextures(1, &m_msDepthBuffer);
        glBindTexture(GL_TEXTURE_2D, m_msDepthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
                    0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                    m_msDepthBuffer, 0);

//         Disable reading to avoid problems with older GPUs
//        glReadBuffer(GL_NONE);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            qDebug()<<"m_msfbo in Framebuffer not complete!"<< Status;
//            return false;
        }

        //  create non-multisampled buffer, to blitt to and read from
//        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glGenTextures(1, &m_colorBuffer);
        glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
          qCritical()<<"m_fbo in Framebuffer not complete!"<< (glCheckFramebufferStatus(GL_FRAMEBUFFER));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

//        // Restore the default framebuffer
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
}

void Framebuffer::bind()
{
//    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);
}

void Framebuffer::readPixel(uint _x, uint _y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    Pixel pixel;
    glReadPixels(_x, _y, 1, 1, GL_RGB, GL_FLOAT, &pixel);

//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

//    qDebug()<<"ReadPixel: "<<Pixel.ObjectID<<Pixel.DrawID<<Pixel.PrimID;
    qDebug()<<"FB ReadPixel: "<<_x<<_y<<pixel.r<<pixel.g<<pixel.b;






//    Color pixel;
//    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    qDebug()<<"ReadPixel: "<<Pixel.ObjectID<<Pixel.DrawID<<Pixel.PrimID;
}

void Framebuffer::debug()
{

}

uint Framebuffer::fbo()
{
    return m_fbo;
}


bool Framebuffer::isMultisample()
{
    return ms;
}

















