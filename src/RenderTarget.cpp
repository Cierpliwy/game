#include "RenderTarget.h"
#include "GameException.h"

RenderTarget::RenderTarget() : m_framebuffer(0)
{
}

RenderTarget::~RenderTarget()
{
    if (m_framebuffer) glDeleteFramebuffers(1, &m_framebuffer);
}

void RenderTarget::create(GLuint target, unsigned int width, unsigned int height)
{
    //Create texture for framebuffer
    m_texture.create(target, width, height);

    //Create framebuffer
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    //Attach texture to framebuffer
    if (m_texture.getTarget() == GL_TEXTURE_2D)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                               GL_TEXTURE_2D, m_texture.id(), 0);
    if (m_texture.getTarget() == GL_TEXTURE_1D)
        glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                               GL_TEXTURE_1D, m_texture.id(), 0);

    //Check status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw GameException(GameException::GL, "framebuffer");

    //Return to default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
