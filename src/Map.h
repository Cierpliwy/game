#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include "GL/glew.h"

struct SDL_Surface;

class Map {
public:
    Map();
    ~Map();
    void load(const char *path);
    void generate(float width, float height);
	void draw();

private:
    void free();
	unsigned char getPixel(unsigned int x, unsigned int y);
	void addPoint(unsigned int x, unsigned int y);

    float m_width;
    float m_height;
    std::string m_path;
    SDL_Surface *m_surface;
	std::vector<GLfloat> m_vertices;

	GLuint m_vao;
	GLuint m_vbo;
};

#endif //MAP_H
