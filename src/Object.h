#pragma once

#include "ObjectLoader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <stdio.h>
#include <string.h>
#include <Box2D/Box2D.h>


using namespace std;

class Object 
{
    //first check simple_polygon for crash - then complex
    b2PolygonShape simple_polygon;
    b2PolygonShape complex_polygon;

    string mesh_path;
    GLuint vbo;
    GLuint vao;
    GLuint uvbuffer;
    glm::vec2 position;

    std::vector<ObjectVertex> vertices;
    Texture texture;

protected:
    b2Body* body;
    const b2World * world;

public:

    Object(){}
    Object(const char* mesh_path);

    //always load mesh before setting physics !!!
    virtual bool loadMesh(const char* mesh_path = NULL);

    //sets physics of the body - position, width, height (if set to 0 it will take measures from mesh, and if it is dynamic, or static)
    virtual void setPhysics(b2World * world, int pos_x, int pos_y, int width = 0, int height = 0, bool dynamic = true);

    //call draw only adter loadMesh and setPhysics !!!
    void draw(GLuint texLocation);

    void setPosition(glm::vec2 &position){this->position = position;}
    glm::vec2 getPosition(){return position;}
    ~Object(void);
};

