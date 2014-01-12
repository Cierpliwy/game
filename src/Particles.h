#pragma once
#include "Object.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define PARTICLES_PER_METER 1
#define RADIUS_OF_PARTICLE 0.5

class Particle : public Object{
    bool mTouched;

public:
    Particle() : mTouched(false){}

    void setPhysics(b2World *world, float x, float y){
        this->world=world;

        b2BodyDef bodyDef;

        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(x, y);
        bodyDef.userData = this;
        bodyDef.fixedRotation = true;
        bodyDef.bullet=true;
        this->body = world->CreateBody(&bodyDef);

        b2CircleShape shape;
        shape.m_radius = RADIUS_OF_PARTICLE;

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density=1.0f;
        fixture.friction = 0.4f;
        fixture.userData = this;
        this->body -> CreateFixture(&fixture);
    }

    bool wasTouched(){return mTouched;}
    void resetTouched(){mTouched = false;}
    void touched(Object * touched_by){mTouched = true;}
    void setPosition(const b2Vec2 &pos){ body->SetTransform(pos,body->GetAngle());}

    ~Particle(){
    }
};

class Particles : public Object{

    b2World *world;
    float pos_x, pos_y, width, height;

    vector<Particle*> particles;

    b2Vec2 generateRandomPosition();
    void generateParticle();
public:
    Particles(void);

    //sets physics of the body - position, width, height (if set to 0 it will 
    // take measures from mesh, and if it is dynamic, or static) - in meters!
    //must be dynamic to work as falling particles
    void setPhysics(b2World * world, float pos_x, float pos_y, 
        float width = 0, float height = 0, bool dynamic = true);

    //call draw only after loadMesh and setPhysics !!!
    void draw();

    ~Particles(void);
};

