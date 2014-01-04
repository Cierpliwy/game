#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H
#include "Texture.h"

class RenderTarget
{
public:
    RenderTarget();
    ~RenderTarget();

    void create(GLuint target, unsigned int width, unsigned int height);

    Texture& getTexture() { return m_texture; }
    GLuint getFramebuffer() { return m_framebuffer; }

private:

    Texture m_texture;
    GLuint m_framebuffer;

};

#endif //RENDER_TARGET_H
