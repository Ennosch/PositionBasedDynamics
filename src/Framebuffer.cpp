#include "include/Framebuffer.h"

#include "Scene.h"

#include <QDebug>
#include <iostream>

struct Pixel {
        float ObjectID;
        float DrawID;
        float PrimID;

        Pixel()     {
            ObjectID = 0.0f;
            DrawID = 0.0f;
            PrimID = 0.0f;
        }
    };

Framebuffer::Framebuffer(Scene *_scene) :
    m_scene(_scene)
{
    WindowWidth = m_scene->width();
    WindowHeight = m_scene->height();
}

bool Framebuffer::init()
{
    bool ms = true;
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create the texture object for the primitive information buffer
        glGenTextures(1, &m_pickingTexture);
        if(ms){ glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_pickingTexture);}
        else{glBindTexture(GL_TEXTURE_2D, m_pickingTexture);}

        if(ms){ glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, WindowWidth, WindowHeight, GL_TRUE);}
        else{glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);}

        if(ms){ glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_pickingTexture, 0);}
        else{glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);}


//         Create the texture object for the depth buffer
        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
                    0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                    m_depthTexture, 0);

//         Disable reading to avoid problems with older GPUs
//        glReadBuffer(GL_NONE);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        // Verify that the FBO is correct
        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//        auto error = glGetError();
//        qDebug()<<"glGetError"<<error;

        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            qDebug()<<"FB error, status: 0x%x\n"<< Status;
            return false;
        }

//        // Restore the default framebuffer
//        glBindTexture(GL_TEXTURE_2D, 0);
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::readPixel(uint _x, uint _y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);


    Pixel Pixel;
    glReadPixels(_x, _y, 1, 1, GL_RGB, GL_FLOAT, &Pixel);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    qDebug()<<"ReadPixel: "<<Pixel.ObjectID<<Pixel.DrawID<<Pixel.PrimID;
}

void Framebuffer::debug()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

        qDebug()<<WindowHeight<<WindowWidth;

    for(int i = 0; i <= WindowHeight; i++)
    {
        for(int j = 0; j<= WindowWidth; j++)
        {
            Pixel Pixel;
            glReadPixels(i, j, 1, 1, GL_RGB, GL_FLOAT, &Pixel);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//            if(Pixel.ObjectID < 0.1 && Pixel.DrawID  0.1 && Pixel.PrimID > 0.1)
            if(Pixel.DrawID > 1.0)
            {
                qDebug()<<"ReadPixel: "<<Pixel.ObjectID<<Pixel.DrawID<<Pixel.PrimID<<i<<j;
                return;
            };
        }
    }


}

uint Framebuffer::fbo()
{
    return m_fbo;
}

















