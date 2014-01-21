#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace std;
#include "Box2D/Box2D.h"
#include "Object.h"

class Player : public Object, public ObjectTouchListener
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    //how many times we can apply force while jumping 
    const static int JUMPING_MAX = 6;
    int jump_moves;
    unsigned int lives;

 
    Object *left_leg, *right_leg, *torso, *head, *left_arm, *right_arm;
    
    float time_interval;
    bool mHasFullBody; 
    bool human_movability;

public:
    Player();

    float setTimeInterval(float time){time_interval=time;}
    float getMass();
    const b2Vec2 getPosition() const {return head->getBody()->GetPosition();}
    void setPV(const glm::mat4 &PV);

    void setLeftLeg(Object *object);
    void setRightLeg(Object *object);
    void setTorso(Object *object);

    void setHead(Object *object);
    void setLeftArm(Object *object);
    void setRightArm(Object *object);

    bool isBodyComplete();
    void setHumanMovability(bool _human_movability) { human_movability = _human_movability;}

    void setPhysics(b2World * world, float pos_x, float pos_y, float width, float height, bool dynamic = true);
    void touched(Object * touched_by);
    void moveRight();
    void moveLeft();
    void jump();
    void draw();
    void loadCharacterFromObj(const char* filename);
    ~Player(void);
};

