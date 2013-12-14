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
    void free();
    GLuint id() { return m_id; }

private:

    SDL_Surface *m_surface;
    std::string m_path;
    GLuint m_id;
};

