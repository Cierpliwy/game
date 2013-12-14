#include "Object.h"
#include <iostream>
using namespace std;

Object::Object(const char* mesh_path){
    strcpy (this->mesh_path,mesh_path);
}

bool Object::loadObject(){

    if(!ObjectLoader::loadSimpleObject(mesh_path, vertices, texture)){
        return false;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ObjectVertex),
                 &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(ObjectVertex::POSITION, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(ObjectVertex), 
                          (void*)offsetof(ObjectVertex, position));
    glEnableVertexAttribArray(ObjectVertex::POSITION);

    glVertexAttribPointer(ObjectVertex::UV, 2, GL_FLOAT, GL_FALSE, 
                          sizeof(ObjectVertex), 
                          (void*)offsetof(ObjectVertex, uv));
    glEnableVertexAttribArray(ObjectVertex::UV);

    glVertexAttribPointer(ObjectVertex::NORMAL, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(ObjectVertex), 
                          (void*)offsetof(ObjectVertex, normal));
    glEnableVertexAttribArray(ObjectVertex::NORMAL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void Object::draw(GLuint texLocation)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id());
    glUniform1i(texLocation, 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

Object::~Object(void)
{
}
