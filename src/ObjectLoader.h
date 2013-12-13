#pragma once

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

class ObjectLoader
{
public:
    ObjectLoader(void);

    static bool loadSimpleObject(const char * path, 
        std::vector<glm::vec3> & out_vertices, 
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals
        );
    ~ObjectLoader(void);
};

