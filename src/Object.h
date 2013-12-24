#pragma once

#include "ObjectLoader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <cfloat>
#include <stdio.h>
#include <string.h>
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Program.h"

//meter to pixels
#define M2P 0.05
//pixels to meters
#define P2M 1/M2P

using namespace std;

class Object 
{
    string mesh_path;
    GLuint vbo;
    GLuint vao;
    GLuint uvbuffer;

    //it helps to scale object to size needed
    glm::vec3 scaler;

    std::vector<ObjectVertex> vertices;
    Texture texture;

protected:
    b2Body* body;
    const b2World * world;

public:
    Object(const char* mesh_path = NULL);

    //always load mesh before setting physics !!!
    virtual bool loadMesh(const char* mesh_path = NULL);

    //sets physics of the body - position, width, height (if set to 0 it will take measures from mesh, and if it is dynamic, or static) - in meters !!!!
    virtual void setPhysics(b2World * world, float pos_x, float pos_y, float width = 0, float height = 0, bool dynamic = true);

    //call draw only adter loadMesh and setPhysics !!!
    void draw(GLuint texLocation, glm::mat4 MVP, GLuint &MVPLocation);

    //void setPosition(glm::vec2 &position){this->position = position;}
    //glm::vec2 getPosition(){return position;}
    ~Object(void);
};

