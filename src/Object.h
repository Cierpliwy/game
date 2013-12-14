#pragma once

#include "ObjectLoader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

class Object
{
    char mesh_path[200];
    GLuint vbo;
    GLuint vao;
    GLuint uvbuffer;
    glm::vec2 position;

    std::vector<ObjectVertex> vertices;
    Texture texture;

public:
    Object(const char* mesh_path);

    bool loadObject();
    void draw(GLuint texLocation);

    void setPosition(glm::vec2 &position){this->position = position;}
    glm::vec2 getPosition(){return position;}
    ~Object(void);
};

