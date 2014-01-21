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
#include "WorldAction.h"
#include "Program.h"

using namespace std;

class Object;
class ObjectTouchListener{
public:
    virtual void touched(Object * touched_by) = 0;
};

//Class that represents actions that can take place while objects will touch
class ObjectAction{

    int value;
    string data;
    bool mSingleAction;
public:
    enum TypeOfAction{
        HEALTH_UP,
        POINT_UP,
        BODY_PART,
        DAMAGE,
        FLOOR_TOUCHED
    };

private:

    TypeOfAction action;

public:

    ObjectAction(TypeOfAction action, bool single_action = false){
        mSingleAction = single_action;
        this->action = action;
        value = 0;
    }
    ObjectAction(TypeOfAction action, int value,bool single_action = false){
        mSingleAction = single_action;
        this->action = action;
        this->value = value;
    }
    ObjectAction(TypeOfAction action, string data,bool single_action = false){
        mSingleAction = single_action;
        this->action = action;
        this->data = data;
    }

    TypeOfAction getAction(){ return action; } 
    int getValue(){ return value; }
    string getData(){ return data; }
    bool isSingleAction(){return mSingleAction;}
};

class Object {

    string mesh_path;
    ObjectTouchListener *listener;
    string class_name;

protected:
    GLuint vbo;
    GLuint vao;
    GLuint uvbuffer;
    GLfloat width, height;

    glm::vec2 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::vector<ObjectVertex> vertices;
    Texture texture;

    WorldActionProvider *world_action_provider;

    const Program *program;
    const glm::mat4 *PV;

    GLuint PVLocation;
    GLuint MLocation;
    GLuint texLocation;

    std::vector<ObjectAction> object_actions;
    b2Body* body;
    b2World * world;

    bool to_delete;

public:
    Object(const char* mesh_path = NULL);

    //getObjectActions while touch something 
    std::vector<ObjectAction> &  getObjectActions(){return object_actions;}

    void deleteObjectActions(){object_actions.empty();}
    void setObjectActions(vector<ObjectAction> actions){object_actions = actions;}

    //always load mesh before setting physics !!!
    virtual bool loadMesh(const char* mesh_path = NULL);

    //sets physics of the body - position, width, height (if set to 0 it will 
    // take measures from mesh, and if it is dynamic, or static) - in meters!
    virtual void setPhysics(b2World * world, float pos_x, float pos_y, 
        float width = 0, float height = 0, 
        bool dynamic = true);

    //call draw only after loadMesh and setPhysics !!!
    virtual void draw();

    virtual void touched(Object * touched_by);
    void setProgram(const Program &program);
    const Program& getProgram() const { return *program;}

    void setObjectName(string _name) {class_name = _name;}
    string getObjectName() {return class_name;}

    void destroy(){to_delete = true;}
    bool isFlagedForDelete(){return to_delete;}

    void setPV(const glm::mat4 &PV) {this->PV = &PV;}
    const glm::mat4& getPV() const {return *PV;}

    virtual const b2Vec2 getPosition() const {return body->GetPosition();}

    void setRotation(const glm::vec3 &rotation){this->rotation = rotation;}
    glm::vec3 getRotation() const {return rotation;}

    b2Body * getBody() {return body;}

    float getHeight(){return height;}
    float getWidth(){return width;}

    void setScale(const glm::vec3 &scale){this->scale = scale;}
    glm::vec3 getScale() const {return scale;}

    Texture & getTexture() { return texture;}

    void setObjectTouchListener(ObjectTouchListener *listener){ this->listener=listener; }
    void setWorldActionProvider(WorldActionProvider *provider){ this->world_action_provider = provider;}

    ~Object(void);
};

