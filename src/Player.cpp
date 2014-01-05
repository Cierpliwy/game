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
    this->body->SetFixedRotation(true);
}
void Player::moveRight(){
    //if(checkIfCrash(glm::vec2(pos.x+velocity.x*time,pos))){

    //}
    b2Vec2 vel = body->GetLinearVelocity();
    float force = 0;
    if(vel.x < 60){
        force = 1400;
    }
    body->ApplyForce(b2Vec2(force,0),body->GetWorldCenter(), true);

}
void Player::moveLeft(){
    b2Vec2 vel = body->GetLinearVelocity();
    float force = 0;
    if(vel.x > -60){
        force = -1400;
    }
    body->ApplyForce(b2Vec2(force,0),body->GetWorldCenter(), true);
}

void Player::jump(){
    if(jump_moves < JUMPING_MAX){
        float force = body->GetMass() * 40; //f = mv/t
        this->body->ApplyLinearImpulse(b2Vec2(0,force), body->GetWorldCenter(),true);
        jump_moves++;
    }
}
Player::~Player(void)
{
}
