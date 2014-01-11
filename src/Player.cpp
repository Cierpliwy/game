#include "Player.h"


Player::Player(char * mesh_path): Object(mesh_path){
}

void Player::touched(Object * touched_by){
    const vector<ObjectAction> &actions = touched_by->getObjectActions();
    for(ObjectAction action : actions){
        if(action.getAction() == ObjectAction::TypeOfAction::TERRAIN_TOUCHED)
            jump_moves = 0;
    }
}

void Player::setPhysics(b2World * world, float pos_x, float pos_y, float width, float height, bool dynamic){
    Object::setPhysics( world, pos_x, pos_y, width, height,dynamic);
    this->body->SetBullet(true);
    //this->body->SetFixedRotation(true);
}
void Player::moveRight(){
    //if(checkIfCrash(glm::vec2(pos.x+velocity.x*time,pos))){

    //}
    //if(rotation.y < 90)
        //rotation.y+=2;
    b2Vec2 vel = body->GetLinearVelocity();
    float force = 0;
    if(vel.x < 60){
        force = body->GetMass()*300;
    }
    body->ApplyForce(b2Vec2(force,0),body->GetWorldCenter(), true);

}
void Player::moveLeft(){
    //if(rotation.y > -90)
        //rotation.y-=2;
    b2Vec2 vel = body->GetLinearVelocity();
    float force = 0;
    if(vel.x > -60){
        force = -(body->GetMass()*300);
    }
    body->ApplyForce(b2Vec2(force,0),body->GetWorldCenter(), true);
}

void Player::jump(){

    if(jump_moves < JUMPING_MAX){
        float force = body->GetMass() * 20; //f = mv/t
        this->body->ApplyLinearImpulse(b2Vec2(0,force), body->GetWorldCenter(),true);
        jump_moves++;
    }
}


void Player::setLeftLeg(Object *object){
    if(left_leg){
        delete left_leg;
    }
}
void Player::setRightLeg(Object *object){
    if(right_leg){
        delete right_leg;
    }
}
void Player::setTorso(Object *object){
    if(torso){
        delete torso;
    }
    torso = object;
    b2JointDef jointDef;

    jointDef.bodyA = torso->getBody();
    const b2Vec2 &position = torso->getBody()->GetPosition();
    //const b2Vec2 &position = torso->getBody()->get.;
    jointDef.bodyB = head->getBody();
    jointDef.collideConnected = false;
    world->CreateJoint(&jointDef);
}
void Player::setHead(Object *object){
    if(head){
        delete head;
    }
}
void Player::setLeftArm(Object *object){
    if(left_arm){
        delete left_arm;
    }
}
void Player::setRightArm(Object *object){
    if(right_arm){
        delete right_arm;
    }
}
Player::~Player(void)
{
}
