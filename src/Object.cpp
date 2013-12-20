#include "Object.h"

void Object::setPhysics(b2World * world, int pos_x, int pos_y, int width, int height, bool dynamic){
    //down_left up_left up_right down_right

    this->world = world;
    b2Vec2 *vertices_tmp = new b2Vec2[4];
    for(ObjectVertex &i : vertices){
        if(vertices_tmp[0].x > i.position.x){
            vertices_tmp[0].x = i.position.x;
            vertices_tmp[1].x = i.position.x;
        }
        if(vertices_tmp[0].y > i.position.y){
            vertices_tmp[0].y = i.position.y;
            vertices_tmp[3].y = i.position.y;
        }
        if(vertices_tmp[2].x < i.position.x){
            vertices_tmp[2].x = i.position.x;
            vertices_tmp[3].x = i.position.x;
        }
        if(vertices_tmp[2].y < i.position.y){
            vertices_tmp[2].y = i.position.y;
            vertices_tmp[1].y = i.position.y;
        }
    }

    int32 count = 4;
    simple_polygon.Set(vertices_tmp, count);
    b2BodyDef bodyDef;
    if(dynamic)
        bodyDef.type = b2_dynamicBody;
    
    bodyDef.position.Set(pos_x, pos_y);
    bodyDef.userData = this;
    bodyDef.fixedRotation = true;
    this->body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    //shape.m_vertices = vertices_tmp;
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density=1.0;
    body -> CreateFixture(&fixture);
}

Object::Object(const char* mesh_path){
    this->mesh_path = mesh_path;
}

bool Object::loadMesh(const char* mesh_path){
    if(mesh_path != NULL){
        this->mesh_path = mesh_path;
    }
    if(this->mesh_path.empty()){
        return false;
    }
    if(!ObjectLoader::loadSimpleObject(this->mesh_path.c_str(), vertices, texture)){
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
    //float32 angle = body->GetAngle();
    //const b2Vec2 &position = body->GetPosition();
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
