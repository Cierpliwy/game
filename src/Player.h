#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace std;

#include "Object.h"

class Player : public Object
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    glm::vec3 velocity;
    glm::vec3 acceleration;
    //bool(*checkIfCrash)(glm::vec2 pos); 

public:
    Player(char * mesh_path);
    void moveRight(float time);
    void moveLeft(float time);
    void jump();
    void loadCharacterFromObj(const char* filename);
    ~Player(void);
};

