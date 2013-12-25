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
    enum ObjectType{
        ENEMY,
        TERRAIN
    };

    ObjectType objectType;
    b2Body* body;
    const b2World * world;

public:
    Object(const char* mesh_path = NULL);

    ObjectType getObjectType(){return objectType;}

    void setObjectType(ObjectType type){objectType = type;}

    //always load mesh before setting physics !!!
    virtual bool loadMesh(const char* mesh_path = NULL);

    //sets physics of the body - position, width, height (if set to 0 it will 
    // take measures from mesh, and if it is dynamic, or static) - in meters!
    virtual void setPhysics(b2World * world, float pos_x, float pos_y, 
        float width = 0, float height = 0, 
        bool dynamic = true);

    //call draw only after loadMesh and setPhysics !!!
    void draw();

    virtual void touched();
    void setProgram(const Program &program);
    const Program& getProgram() const { return *program;}

    void setPV(const glm::mat4 &PV) {this->PV = &PV;}
    const glm::mat4& getPV() const {return *PV;}

    const b2Vec2 getPosition() const {return body->GetPosition();}

    void setRotation(const glm::vec3 &rotation){this->rotation = rotation;}
    glm::vec3 getRotation() const {return rotation;}

    void setScale(const glm::vec3 &scale){this->scale = scale;}
    glm::vec3 getScale() const {return scale;}

    ~Object(void);
};

