#include "CollisionInterface.h"

CollisionInterface::TypeOfCollision CollisionInterface::checkCollision(glm::vec2 up_left, glm::vec2 down_right){
    //axe X collision
    if(down_right.x >= this->up_left.x && up_left.x <= this->down_right.x){ 
        //axe Y collision 
        if(up_left.y >= this->down_right.y && down_right.y <= this->up_left.y){ 
            return CollisionInterface::TypeOfCollision::COLLISION;
        } 
    }  

    return CollisionInterface::TypeOfCollision::NO_COLLISION;
}
