#include "Map.h"
#include "GameException.h"
#include "SDL_image.h"
#include <iostream>
using namespace std;

Map::Map() : m_width(0), m_height(0), m_surface(NULL), m_vao(0), m_vbo(0){
}

Map::~Map(){
    free();
}

void Map::free(){
    if (m_surface) SDL_FreeSurface(m_surface);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
}

void Map::load(const char *path){
    m_path = path;
    m_path += "_collision.png";
    free();

    m_surface = IMG_Load(m_path.c_str());
    if (!m_surface) 
        throw GameException(GameException::SDL_IMAGE, m_path.c_str());

    std::string texPath = path;
    texPath += "_texture.png";
    m_texture.load(texPath.c_str());
}

unsigned char Map::getPixel(unsigned int x, unsigned int y){
    unsigned char bpp = m_surface->format->BytesPerPixel;
    return !static_cast<unsigned char*>(m_surface->pixels)
                                      [y * m_surface->pitch + x * bpp];
}

void Map::addPoint(unsigned int x, unsigned int y, float z){
    float pw = m_width / m_surface->w;
    float ph = m_height / m_surface->h;
    MapVertex v;
    v.uv.x = x / static_cast<float>(m_surface->w);
    v.uv.y = y / static_cast<float>(m_surface->h);
    v.pos.x = x*pw;
    v.pos.y = -1*(y*ph);
    v.pos.z = z;
    m_vertices.push_back(v);
}

void Map::generate(float width, float depth) {
    m_width = width;
    m_height = width * static_cast<float>(m_surface->h) / m_surface->w;
    m_vertices.clear();

    for(int i = 0; i < m_surface->w; ++i) {
        for(int j = 0; j < m_surface->h; ++j) {

            //Check neighbours
            unsigned int mask = 0;
            unsigned int dir = 0;
            if (j-1 >= 0 && getPixel(i, j-1)) {mask |= 12; dir |= 1;}
            if (j+1 < m_surface->h && getPixel(i, j+1)) {mask |= 3; dir |= 2;}
            if (i-1 >= 0 && getPixel(i-1, j)) {mask |= 9; dir |= 4;}
            if (i+1 < m_surface->w && getPixel(i+1, j)) {mask |= 6; dir |= 8;}

            if (getPixel(i,j)) {

                addPoint(i,j,0);
                addPoint(i+1,j,0);
                addPoint(i+1,j+1,0);
                addPoint(i+1,j+1,0);
                addPoint(i,j+1,0);
                addPoint(i,j,0);

                //Add collision lines
                if (!(dir & 1)) m_lines.push_back(glm::vec4(i,j,i+1,j));
                if (!(dir & 2)) m_lines.push_back(glm::vec4(i,j+1,i+1,j+1));
                if (!(dir & 4)) m_lines.push_back(glm::vec4(i,j+1,i,j));
                if (!(dir & 8)) m_lines.push_back(glm::vec4(i+1,j,i+1,j+1));
                
            } else {

                if (mask == 14 || mask == 13 || mask == 11 || mask == 7) {
                    if (mask & 8) addPoint(i,j,0);
                    if (mask & 4) addPoint(i+1,j,0);
                    if (mask & 2) addPoint(i+1,j+1,0);
                    if (mask & 1) addPoint(i,j+1,0);

                    if ((mask & 10) == 10)
                        m_lines.push_back(glm::vec4(i,j,i+1,j+1));

                    if ((mask & 5) == 5)
                        m_lines.push_back(glm::vec4(i+1,j,i,j+1));
                }
            }
        }
    }

    // Generate floor
    for(unsigned int i=0; i < m_lines.size(); ++i) {
        glm::vec4 p = m_lines[i];
        addPoint(p.x, p.y, 0);
        addPoint(p.z, p.w, 0);
        addPoint(p.z, p.w, depth);
        addPoint(p.z, p.w, depth);
        addPoint(p.x, p.y, depth);
        addPoint(p.x, p.y, 0);
    }

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
}

void Map::draw(GLuint texLocation)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture.id());
    glUniform1i(texLocation, GL_TEXTURE0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
}
