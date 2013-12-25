#include "Map.h"
#include "GameException.h"
#include "SDL_image.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace glm;

Map::Map() : m_width(0), m_height(0), m_vao(0), m_vbo(0),
    m_gridVao(0), m_gridVbo(0), m_surface(NULL)
{
}

Map::~Map(){
    free(true);
}

void Map::free(bool freeSurface){
    if (freeSurface && m_surface) SDL_FreeSurface(m_surface);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_gridVao) glDeleteVertexArrays(1, &m_gridVao);
    if (m_gridVbo) glDeleteBuffers(1, &m_gridVbo);
    m_vertices.clear();
    m_gridVertices.clear();
    m_lines.clear();
}

void Map::init() {
    // Prepare shaders
    m_vertexShader.load("../data/maps/mapVertex.glsl");
    m_vertexShader.compile();
    m_fragmentShader.load("../data/maps/mapFragment.glsl");
    m_fragmentShader.compile();
    m_program.create();
    m_program.setFragmentShader(m_fragmentShader);
    m_program.setVertexShader(m_vertexShader);
    m_program.link();
     
    // Get uniform locations
    m_PVLocation = m_program.getUniformLocation("PV");
    m_lightPosLocation = m_program.getUniformLocation("light");
    m_lightSizeLocation = m_program.getUniformLocation("lightSize");
    m_texture0Location = m_program.getUniformLocation("tex0");
    m_texture1Location = m_program.getUniformLocation("tex1");
    m_visibilityLocation = m_program.getUniformLocation("vis");
    m_enableGridLocation = m_program.getUniformLocation("grid");
}

void Map::load(const char *path){

    // Load collision map
    m_path = path;
    m_path += ".png";
    free(true);

    m_surface = IMG_Load(m_path.c_str());
    if (!m_surface || !m_surface->format->palette) 
        throw GameException(GameException::SDL_IMAGE, m_path.c_str());

    // Load sprites textures
    for(unsigned int i = 0; i < 3; ++i) {
        std::string texPath = path;
        stringstream ss;
        ss << path << "_bg" << i << ".png";
        m_sprites[i].load(ss.str().c_str());
    }

    // Load map texture
    string texPath = path;
    texPath += "_tex.png";
    m_texture.load(texPath.c_str());

    // Load gfx texture
    texPath = path;
    texPath += "_gfx.png";
    m_gfx.load(texPath.c_str());
}

unsigned char Map::getPixel(unsigned int x, unsigned int y){
    unsigned char bpp = m_surface->format->BytesPerPixel;
    unsigned char i = static_cast<unsigned char*>(m_surface->pixels)
        [y * m_surface->pitch + x * bpp];
    return !m_surface->format->palette->colors[i].r;
}

vec2 Map::getPos(unsigned int x, unsigned int y)
{
    vec2 pos;
    pos.x = x * m_width / m_surface->w;
    pos.y = (m_surface->h - y) * m_height / m_surface->h; 
    return pos;
}

glm::vec2 Map::getUV(unsigned int x, unsigned int y)
{
    // Remember that V or Y is flipped by loading from SDL
    vec2 pos;
    pos.x = x / static_cast<float>(m_surface->w);
    pos.y = y / static_cast<float>(m_surface->h); 
    return pos;
}

void Map::addFilledTriangle(unsigned int x1, unsigned int y1,
                            unsigned int x2, unsigned int y2,
                            unsigned int x3, unsigned int y3)
{
    MapVertex v1,v2,v3;
    v1.pos = vec3(getPos(x1,y1),0.0f);
    v2.pos = vec3(getPos(x2,y2),0.0f);
    v3.pos = vec3(getPos(x3,y3),0.0f);
    v1.uv = getUV(x1,y1);
    v2.uv = getUV(x2,y2);
    v3.uv = getUV(x3,y3);
    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
}

void Map::addFloor(unsigned int x1, int unsigned y1,
                   unsigned int x2, int unsigned y2,
                   float uvx, float uvy)
{
    MapVertex v1,v2,v3,v4;
    v1.pos = vec3(getPos(x1,y1),0.0f);
    v2.pos = vec3(getPos(x2,y2),0.0f);
    v1.uv = getUV(x1,y1);
    v2.uv = getUV(x2,y2);
    v3 = v2;
    v4 = v1;
    v3.pos.z = m_depth;
    v4.pos.z = m_depth;

    // Update UVs for far points
    float sqrSize = m_width / m_surface->w;
    v3.uv.x += uvx * m_uvFix * sqrSize;
    v3.uv.y += uvy * m_uvFix * sqrSize;
    v4.uv.x += uvx * m_uvFix * sqrSize;
    v4.uv.y += uvy * m_uvFix * sqrSize;

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
    m_vertices.push_back(v3);
    m_vertices.push_back(v4);
    m_vertices.push_back(v1);
}

#include "Windows.h"
#include "WinBase.h"
void Map::setPhysics(b2World * world){

    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(0, 0); //middle, bottom
    body = world->CreateBody(&myBodyDef);

    //b2PolygonShape polygonShape;
    b2EdgeShape edgeShape;
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &edgeShape;
    myFixtureDef.density = 1;
    
    for(Line<glm::vec2> line : m_lines){
        edgeShape.Set(b2Vec2(line.a.x*P2M,-line.a.y*P2M),b2Vec2(line.b.x*P2M,-line.b.y*P2M)); //adding line
        //OutputDebugString("\nVec2: ");
        //OutputDebugString(std::to_string(line.a.x*P2M).c_str());
        //OutputDebugString(" : ");
        //OutputDebugString(std::to_string(line.a.y*P2M).c_str());
        //OutputDebugString(" Vec22: ");
        //OutputDebugString(std::to_string(line.b.x*P2M).c_str());
        //OutputDebugString(" : ");
        //OutputDebugString(std::to_string(line.b.y*P2M).c_str());
        body->CreateFixture(&myFixtureDef); //add a fixture to the body
    } 

    //floor below map
    edgeShape.Set(b2Vec2(-200,30),b2Vec2(200,30)); //adding line
    body->CreateFixture(&myFixtureDef); //add a fixture to the body 
}

void Map::generate(float width, float depth, float uvFix) 
{
    // Clear all data excluding bitmaps
    free(false);

    // Get ratio from bitmap
    m_width = width;
    m_height = width * static_cast<float>(m_surface->h) / m_surface->w;
    m_depth = depth;
    m_uvFix = uvFix;

    // Generate sprites
    float sprDepth = 0.9f;
    for(unsigned int i = 0; i < 3; ++i) {
        m_sprites[i].generate(Rect<vec3>(
            vec3(0.0f, 0.0f, sprDepth*depth),
            vec3(m_width, 0.0f, sprDepth*depth),
            vec3(m_width, m_height, sprDepth*depth),
            vec3(0.0f, m_height, sprDepth*depth)));
        sprDepth -= 0.4f;
    }

    // Process pixels
    for(int i = 0; i < m_surface->w; ++i) {
        for(int j = 0; j < m_surface->h; ++j) {

            // Check neighbours
            unsigned int sqrPts = 0;
            unsigned int dir = 0;
            if (j-1 >= 0 && getPixel(i, j-1)) {
                sqrPts |= UPLEFT | UPRIGHT; 
                dir |= UP;
            }
            if (j+1 < m_surface->h && getPixel(i, j+1)) {
                sqrPts |= DOWNLEFT | DOWNRIGHT; 
                dir |= DOWN;
            }
            if (i-1 >= 0 && getPixel(i-1, j)) 
            {
                sqrPts |= UPLEFT | DOWNLEFT; 
                dir |= LEFT;
            }
            if (i+1 < m_surface->w && getPixel(i+1, j)) {
                sqrPts |= UPRIGHT | DOWNRIGHT;
                dir |= RIGHT;
            }

            // If pixel is filled just add square to map
            if (getPixel(i,j)) {

                addFilledTriangle(i,j,i+1,j,i+1,j+1);
                addFilledTriangle(i+1,j+1,i,j+1,i,j);

                // Add collision lines
                if (!(dir & UP)) 
                    m_lines.push_back(Line<vec2>(vec2(i,j), vec2(i+1,j)));
                if (!(dir & DOWN)) 
                    m_lines.push_back(Line<vec2>(vec2(i+1,j+1), vec2(i,j+1)));
                if (!(dir & LEFT)) 
                    m_lines.push_back(Line<vec2>(vec2(i,j+1), vec2(i,j)));
                if (!(dir & RIGHT)) 
                    m_lines.push_back(Line<vec2>(vec2(i+1,j), vec2(i+1,j+1)));

            } else {
                // Try to anti alias edges
                vector<vec2> points;
                if (sqrPts & UPLEFT) points.push_back(vec2(i,j));
                if (sqrPts & UPRIGHT) points.push_back(vec2(i+1,j));
                if (sqrPts & DOWNRIGHT) points.push_back(vec2(i+1,j+1));
                if (sqrPts & DOWNLEFT) points.push_back(vec2(i,j+1));
                if (points.size() == 3) {
                    //We can add filling rectangle to smooth it out.
                    addFilledTriangle(points[0].x, points[0].y,
                        points[1].x, points[1].y,
                        points[2].x, points[2].y);

                    // Add collision lines
                    m_lines.push_back(Line<vec2>(points[0], points[1]));
                    m_lines.push_back(Line<vec2>(points[1], points[2]));
                    m_lines.push_back(Line<vec2>(points[2], points[0]));
                }
            }
        }
    }

    // Generate floor from collision lines and transform them to
    // local map coordinates.
    for(unsigned int i = 0; i < m_lines.size(); ++i) {
        Line<vec2> l = m_lines[i];
        float uvx = 0.0f, uvy = 0.0f;

        if (l.a.x < l.b.x) uvy = -1.0f; 
        if (l.a.x > l.b.x) uvy = 1.0f; 
        if (l.a.y > l.b.y) uvx = -1.0f;
        if (l.a.y < l.b.y) uvx = 1.0f;

        addFloor(l.a.x, l.a.y, l.b.x, l.b.y, uvx, uvy);

        m_lines[i].a = getPos(l.a.x, l.a.y);
        m_lines[i].b = getPos(l.b.x, l.b.y);
    }

    // Sending data to OpenGL
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(MapVertex),
        &m_vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(MapVertex::POS, 3, GL_FLOAT, GL_FALSE, 
        sizeof(MapVertex), (void*)offsetof(MapVertex, pos));
    glEnableVertexAttribArray(MapVertex::POS);

    glVertexAttribPointer(MapVertex::UV, 2, GL_FLOAT, GL_FALSE, 
        sizeof(MapVertex), (void*)offsetof(MapVertex, uv));
    glEnableVertexAttribArray(MapVertex::UV);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Grid data for OpenGL
    for(unsigned int i = 0; i < m_lines.size(); ++i) {
        m_gridVertices.push_back(vec3(m_lines[i].a,0.0f));
        m_gridVertices.push_back(vec3(m_lines[i].b,0.0f));
    }

    glGenVertexArrays(1, &m_gridVao);
    glBindVertexArray(m_gridVao);

    glGenBuffers(1, &m_gridVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVbo);

    glBufferData(GL_ARRAY_BUFFER, m_gridVertices.size() * sizeof(vec3),
        &m_gridVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Map::draw(unsigned int target)
{
    m_program.use();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gfx.id());
    glUniform1i(m_texture1Location, 1);

    glUniformMatrix4fv(m_PVLocation, 1, GL_FALSE, value_ptr(*m_PV));
    glUniform2f(m_lightPosLocation, m_lightPos->x, m_lightPos->y); 
    glUniform1f(m_lightSizeLocation, m_lightSize);
    glUniform1f(m_visibilityLocation, m_visibility);
    glUniform1ui(m_enableGridLocation, 0);

    if (target & MAP) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture.id());
        glUniform1i(m_texture0Location, 0);


        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        glBindVertexArray(0);
    }

    if (target & SPRITES) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for(unsigned int i = 0; i < 3; ++i) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_sprites[i].getTextureID());
            glUniform1i(m_texture0Location, 0);
            m_sprites[i].draw();
        }

        glDisable(GL_BLEND);
    }

    if (target & GRID) {
        glUniform1ui(m_enableGridLocation, 1);
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(m_gridVao);
        glDrawArrays(GL_LINES, 0, m_gridVertices.size()); 
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
}
