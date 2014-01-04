#pragma once
#include <string>
#include <GL/glew.h>

struct SDL_Surface;

class Texture
{
public:
    Texture(void);
	~Texture(void);

	void load(const char *filePath);
    void create(GLuint target, unsigned w, unsigned h = 1);

    void free();
    GLuint id() { return m_id; }

    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }
    GLuint getTarget() const {return m_target;}

private:

    SDL_Surface *m_surface;
    std::string m_path;
    GLuint m_id;
    unsigned int m_width, m_height;
    GLuint m_target;
};

