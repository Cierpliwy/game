#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Utils.h"
#include "Sprite.h"
#include "Program.h"
struct SDL_Surface;

// Class designed for generating and displaying map from
// bitmaps.
class Map {
public:

    Map();
    ~Map();

    // Initialize
    void init();

    // Load map from path
    void load(const char *path);
    
    // Generate map of specified size
    void generate(float width, float depth, float uvFix);

    // Draw map
    void draw(unsigned int target);

    // Free all map resources
    void free(bool freeSurface);

    // Possible draw targets
    enum DrawTarget {
        MAP = 1,
        SPRITES = 2,
        GRID = 4
    };

    // Set view perspective matrix
    void setPV(const glm::mat4 &PV) {m_PV = &PV;}

    // Set light position
    void setLightPos(const glm::vec2 &pos) {m_lightPos = &pos;}

    // Set light size
    void setLightSize(float size) {m_lightSize = size;}

    // Set visibility
    void setVisibility(float visibility) {m_visibility = visibility;}

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
    GLuint m_gridVao;
    GLuint m_gridVbo;
    GLuint m_texSampleLocation;
    std::vector<MapVertex> m_vertices;
    std::vector<glm::vec3> m_gridVertices;

    // Programs
    Program m_program;
    VertexShader m_vertexShader;
    FragmentShader m_fragmentShader;

    // Draw variables
    const glm::mat4 *m_PV;
    const glm::vec2 *m_lightPos;
    float m_lightSize;
    float m_visibility;

    // Uniform locations
    GLuint m_PVLocation;
    GLuint m_lightPosLocation;
    GLuint m_lightSizeLocation;
    GLuint m_texture0Location;
    GLuint m_texture1Location;
    GLuint m_visibilityLocation;
    GLuint m_enableGridLocation;

    // Textures
    SDL_Surface *m_surface;
    Texture m_texture;
    Texture m_gfx;
    Sprite m_sprites[3];
    
    // Collision data
    std::vector<Line<glm::vec2> > m_lines;

    std::vector<Object> objects;
};

#endif //MAP_H
