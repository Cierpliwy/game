#include "Particles.h"


Particles::Particles(void)
{
}

void Particles::setPhysics(b2World * world, float pos_x, float pos_y, float width, float height , bool dynamic){
    this->world = world;
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->width = width;
    this->height = height;

    int32 count = 4;
    this->world = world;
    glm::vec3 *vertices_tmp = new glm::vec3[count];

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


    scale.x = (RADIUS_OF_PARTICLE*2)/(vertices_tmp[2].x - vertices_tmp[0].x);


    scale.y = (RADIUS_OF_PARTICLE*2)/(vertices_tmp[2].y - vertices_tmp[0].y);

    scale.z = scale.y;


    for(int i = 0; i < width*height*PARTICLES_PER_METER ; ++i){
        generateParticle();
    }
}

void Particles::generateParticle(){
    //S³aby ale random
    srand(time(NULL));
    float x = pos_x + (rand() % static_cast<int>(width));
    float y = pos_y + (rand() % static_cast<int>(height));
    Particle particle;
    particle.setPhysics(world,x,y);
    particles.push_back(particle);
}

void Particles::draw(Texture *customtexture){

    for(vector<Particle>::iterator &it = particles.begin(); it != particles.end(); it++){
        //usuwamy dotkniete
        if(it->wasTouched()){
            it = particles.erase(it);
            generateParticle();
        }


        if (!program || !PV) return;

        // Calculate model matrix
        glm::mat4 model(1.0f);

        const b2Vec2 &pos = it->getPosition();
        model = glm::translate(model, glm::vec3(pos.x, pos.y,0));


        program->use();
        glUniformMatrix4fv(PVLocation, 1, GL_FALSE, glm::value_ptr(*PV));
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        if (customtexture)
            glBindTexture(GL_TEXTURE_2D, customtexture->id());
        else 
            glBindTexture(GL_TEXTURE_2D, texture.id());
        glUniform1i(texLocation, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }
}

Particles::~Particles(void)
{
}
