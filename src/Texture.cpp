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

void Texture::load(const char *filePath, unsigned w, unsigned h)
{
    if (!m_path.empty()) free();
    m_path = filePath;

    if (filePath) {
        m_surface = IMG_Load(filePath);
        if (!m_surface) 
            throw GameException(GameException::SDL_IMAGE, filePath);
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (filePath) {
        if (m_surface->format->BytesPerPixel == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_surface->w, m_surface->h,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, m_surface->pixels);
        else if (m_surface->format->BytesPerPixel == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_surface->w, m_surface->h,
                         0, GL_RGB, GL_UNSIGNED_BYTE, m_surface->pixels);
        else throw GameException(GameException::INTERNAL, "Wrong format"); 
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    if (filePath) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::free()
{
    if (m_surface) {
        SDL_FreeSurface(m_surface);
        m_surface = NULL;
    }

    if (m_id) glDeleteTextures(1, &m_id);
}

