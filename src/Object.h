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
    virtual void createPhysics(b2World * world);

protected:
    b2Body* body;

public:

    Object(){}
    Object(const char* mesh_path);

    virtual bool loadObject(b2World * world, const char* mesh_path = NULL);
    void draw(GLuint texLocation);

    void setPosition(glm::vec2 &position){this->position = position;}
    glm::vec2 getPosition(){return position;}
    ~Object(void);
};

