#include "Player.h"


Player::Player(char * mesh_path): Object(mesh_path){
}

bool Player::loadObject(b2World * world, const char* mesh_path){
    if(!Object::loadObject(world,mesh_path)){
        return false;
    }

    // a bullet so it doesn't fall through the world on huge updates
    body->SetBullet(true);

    return true;
}

void Player::moveRight(float time){
    //if(checkIfCrash(glm::vec2(pos.x+velocity.x*time,pos))){

    //}

}
void Player::moveLeft(float time){

}
void Player::jump(){

}
Player::~Player(void)
{
}
