#include "Object.h"


//#include "Windows.h"
//#include "WinBase.h"


void Object::setPhysics(b2World * world, float pos_x, float pos_y, float width, float height, bool dynamic){
    //down_left up_left up_right down_right

    this->world = world;
    glm::vec3 *vertices_tmp = new glm::vec3[4];

    vertices_tmp[0].x = FLT_MAX;
    vertices_tmp[2].x = FLT_MIN;
    vertices_tmp[0].y = FLT_MAX;
    vertices_tmp[2].y = FLT_MIN;

    for(ObjectVertex &i : vertices){
        if(vertices_tmp[0].x > i.position.x){
            vertices_tmp[0].x = i.position.x;
            vertices_tmp[3].x = i.position.x;
        }
        if(vertices_tmp[0].y > i.position.y){
            vertices_tmp[0].y = i.position.y;
            vertices_tmp[2].y = i.position.y;
        }
        if(vertices_tmp[2].x < i.position.x){
            vertices_tmp[2].x = i.position.x;
            vertices_tmp[1].x = i.position.x;
        }
        if(vertices_tmp[2].y < i.position.y){
            vertices_tmp[2].y = i.position.y;
            vertices_tmp[3].y = i.position.y;
        }
    }

    if(width){
        scaler.x = (width*M2P)/(vertices_tmp[2].x - vertices_tmp[0].x);
    }
    if(height){
        scaler.y = (height*M2P)/(vertices_tmp[2].y - vertices_tmp[0].y);
    }
    scaler.z = scaler.y;

    
    //OutputDebugString("\nChujjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj\n");
    //OutputDebugString(std::to_string(width*M2P).c_str());
    ///OutputDebugString("\n");
    //OutputDebugString(std::to_string(scaler.y).c_str());


    b2Vec2 vertices_to_shape[4];
    vertices_to_shape[0].x = vertices_tmp[0].x*P2M;
    vertices_to_shape[0].y = vertices_tmp[0].y*P2M;
    vertices_to_shape[1].x = vertices_tmp[1].x*P2M;
    vertices_to_shape[1].y = vertices_tmp[1].y*P2M;
    vertices_to_shape[2].x = vertices_tmp[2].x*P2M;
    vertices_to_shape[2].y = vertices_tmp[2].y*P2M;
    vertices_to_shape[3].x = vertices_tmp[3].x*P2M;
    vertices_to_shape[3].y = vertices_tmp[3].y*P2M;

    //create cube that will lock figure inside
    b2PolygonShape simple_polygon;
    int32 count = 4;

    //vertices_to_shape must be CCW (counter clock direction)!!!
    //simple_polygon.SetAsArray(vertices_to_shape, count);
    //simple_polygon.
    //simple_polygon.Set(vertices_to_shape, count);

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
    //body -> CreateFixture(&fixture);
}

Object::Object(const char* mesh_path): scaler(0,0,0){
    if(mesh_path){
        this->mesh_path = mesh_path;
    }
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

void Object::draw(GLuint texLocation , glm::mat4 MVP, GLuint MVPLocation)
{
    //float32 angle = body->GetAngle();
    //const b2Vec2 &position = body->GetPosition();

    //    for(int i = 0; i < vertices.size() ; ++i){
    //      const b2Vec2 & tmp_point = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(i);
    //    vertices[i].position.x = tmp_point.x*P2M;
    //  vertices[i].position.y = tmp_point.y*P2M;
    //}


    MVP = glm::scale(MVP, scaler);
    //MVP = glm::rotate(MVP, angle, glm::vec3(1.0f,1.0f,0.0f));
    MVP = glm::rotate(MVP, 0.1F, glm::vec3(1.0f,1.0f,0.0f));
    glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

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
