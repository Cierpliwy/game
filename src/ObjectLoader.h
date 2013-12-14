#pragma once

#include <vector>
#include <stdio.h>
#include "Texture.h"
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#define PATH_TO_DATA "../data/"
struct ObjectVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    enum Attribiute {
        POSITION,
        UV,
        NORMAL
    };
};

class ObjectLoader
{
public:
    ObjectLoader(void);

    static bool loadSimpleObject(const char * path, 
                                 std::vector<ObjectVertex> &vertices, Texture &texture);
    ~ObjectLoader(void);
};

