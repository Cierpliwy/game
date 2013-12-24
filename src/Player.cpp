#include "Player.h"


Player::Player(char * mesh_path): Object(mesh_path){
}

void Player::setPhysics(b2World * world, float pos_x, float pos_y, float width, float height, bool dynamic){
    Object::setPhysics( world, pos_x, pos_y, width, height,dynamic);
    this->body->SetBullet(true);
    this->body->SetFixedRotation(true);
}
void Player::moveRight(float time){
    //if(checkIfCrash(glm::vec2(pos.x+velocity.x*time,pos))){

    //}

}
void Player::moveLeft(float time){

}

void Player::jump(){
    if(jump_moves < JUMPING_MAX){
        float force = body->GetMass() * 1000 ; //f = mv/t
        this->body->ApplyLinearImpulse(b2Vec2(0,-force), body->GetWorldCenter(),true);
        jump_moves++;
    }
}
Player::~Player(void)
{
}
