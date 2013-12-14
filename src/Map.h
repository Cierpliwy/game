#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"

struct SDL_Surface;

class Map {
public:
    Map();
    ~Map();
    void load(const char *path);
    void generate(float width);
	void draw(GLuint texLocation);

private:

    struct MapVertex {
        glm::vec3 pos;
        glm::vec2 uv;

        enum Attribiutes {
            POS,
            UV
        };
    };

    void free();
	unsigned char getPixel(unsigned int x, unsigned int y);
	void addPoint(unsigned int x, unsigned int y);

    float m_width;
    float m_height;
    std::string m_path;
    SDL_Surface *m_surface;
    std::vector<MapVertex> m_vertices;
    Texture m_texture;
    GLuint m_texSampleLocation;

	GLuint m_vao;
	GLuint m_vbo;
};

#endif //MAP_H
