#include "Object.h"


Object::Object(const char* mesh_path){
    strcpy (this->mesh_path,mesh_path);
}

bool Object::loadObject(){

    if(!ObjectLoader::loadSimpleObject(mesh_path, vertices, uvs, normals)){
        return false;
    }
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 &vertices[0], GL_STATIC_DRAW);

    return true;
}

void Object::draw(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(0);

}

Object::~Object(void)
{
}
