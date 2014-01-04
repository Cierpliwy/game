#include "Texture.h"
#include "GameException.h"
#include <SDL_image.h>
#include "GameException.h"
#include <iostream>
using namespace std;

Texture::Texture(void)
{
    m_surface = NULL;
    m_id = 0;
}

Texture::~Texture(void)
{
    free();
}

void Texture::load(const char *filePath)
{
    free();

    m_path = filePath;
    m_surface = IMG_Load(filePath);
    if (!m_surface) 
        throw GameException(GameException::SDL_IMAGE, filePath);
    m_width = m_surface->w;
    m_height = m_surface->h;
    m_target = GL_TEXTURE_2D;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_surface->format->BytesPerPixel == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_surface->w, m_surface->h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, m_surface->pixels);
    else if (m_surface->format->BytesPerPixel == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_surface->w, m_surface->h,
                     0, GL_RGB, GL_UNSIGNED_BYTE, m_surface->pixels);
    else throw GameException(GameException::INTERNAL, "Wrong format"); 

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::create(GLuint target, unsigned w, unsigned h)
{
    free();
    m_width = w;
    m_height = h;
    m_target = target;

    glGenTextures(1, &m_id);

    glBindTexture(target, m_id);
    if (target == GL_TEXTURE_2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    if (target == GL_TEXTURE_1D) {
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, w, 0, GL_RGBA, 
                     GL_FLOAT, 0);
    }

    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(target, 0);
}

void Texture::free()
{
    if (m_surface) {
        SDL_FreeSurface(m_surface);
        m_surface = NULL;
    }

    if (m_id) glDeleteTextures(1, &m_id);
}

