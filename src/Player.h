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

class Player : public Object
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    //how many times we can apply force while jumping 
    const static int JUMPING_MAX = 6;
    int jump_moves;
    glm::vec3 velocity;
    glm::vec3 acceleration;

public:
    Player(){jump_moves = 0;}
    Player(char * mesh_path);
    void setPhysics(b2World * world, float pos_x, float pos_y, float width, float height, bool dynamic = true);
    void moveRight(float time);
    void moveLeft(float time);
    void jump();
    void loadCharacterFromObj(const char* filename);
    ~Player(void);
};

