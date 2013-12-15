#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Utils.h"
#include "Sprite.h"

struct SDL_Surface;

// Class designed for generating and displaying map from
// bitmaps.
class Map {
public:

    Map();
    ~Map();

    // Load map from path
    void load(const char *path);
    
    // Generate map of specified size
    void generate(float width, float depth, float uvFix);

    // Draw map
	void draw(GLuint texLocation);

    // Free all map resources
    void free();

private:

    // Map vertex
    struct MapVertex {
        glm::vec3 pos;
        glm::vec2 uv;

        enum Attribiutes {
            POS,
            UV
        };
    };

    enum SquarePoints {
        UPLEFT = 1,
        UPRIGHT = 2,
        DOWNRIGHT = 4,
        DOWNLEFT = 8
    };

    enum Directions {
        UP = 1,
        DOWN = 2,
        LEFT = 4,
        RIGHT = 8
    };

    // Get pixel from collision bitmap
	unsigned char getPixel(unsigned int x, unsigned int y);
    // Translate pixel position to map local coordinates
    glm::vec2 getPos(unsigned int x, unsigned int y);
    // Translate pixel position to map texture uv coordinates
    glm::vec2 getUV(unsigned int x, unsigned int y);

    // Add filled triangle
    void addFilledTriangle(unsigned int x1, unsigned int y1,
                           unsigned int x2, unsigned int y2,
                           unsigned int x3, unsigned int y3);

    // Add floor
    void addFloor(unsigned int x1, int unsigned y1,
                  unsigned int x2, int unsigned y2,
                  float uvx, float uvy);

	void addPoint(unsigned int x, unsigned int y, float z, int dir = 0);

    // Input parameters
    float m_width;
    float m_height;
    float m_depth;
    float m_uvFix;
    std::string m_path;

    // OpenGL data
	GLuint m_vao;
	GLuint m_vbo;
    GLuint m_texSampleLocation;
    std::vector<MapVertex> m_vertices;

    // Textures
    SDL_Surface *m_surface;
    Texture m_texture;
    Sprite m_sprites[3];
    
    // Collision data
    std::vector<Line<glm::vec2> > m_lines;
};

#endif //MAP_H
