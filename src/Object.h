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
#define M2P 0.1
//pixels to meters
#define P2M 1/M2P

using namespace std;

class Object 
{
    string mesh_path;
    GLuint vbo;
    GLuint vao;
    GLuint uvbuffer;

    glm::vec2 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::vector<ObjectVertex> vertices;
    Texture texture;

    const Program *program;
    const glm::mat4 *PV;

    GLuint PVLocation;
    GLuint MLocation;
    GLuint texLocation;

protected:
    b2Body* body;
    const b2World * world;

public:
    Object(const char* mesh_path = NULL);

    //always load mesh before setting physics !!!
    virtual bool loadMesh(const char* mesh_path = NULL);

    //sets physics of the body - position, width, height (if set to 0 it will 
    // take measures from mesh, and if it is dynamic, or static) - in meters!
    virtual void setPhysics(b2World * world, float pos_x, float pos_y, 
                            float width = 0, float height = 0, 
                            bool dynamic = true);

    //call draw only after loadMesh and setPhysics !!!
    void draw();

    void setProgram(const Program &program);
    const Program& getProgram() const { return *program;}

    void setPV(const glm::mat4 &PV) {this->PV = &PV;}
    const glm::mat4& getPV() const {return *PV;}

    void setPosition(const glm::vec2 &position){this->position = position;}
    glm::vec2 getPosition() const {return position;}
    
    void setRotation(const glm::vec3 &rotation){this->rotation = rotation;}
    glm::vec3 getRotation() const {return rotation;}

    void setScale(const glm::vec3 &scale){this->scale = scale;}
    glm::vec3 getScale() const {return scale;}

    ~Object(void);
};

