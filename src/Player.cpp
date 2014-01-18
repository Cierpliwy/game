#include "Player.h"


Player::Player(){
    jump_moves = 0; 
    lives = 100;
    mHasFullBody = false;
    left_leg = right_leg = torso =head =left_arm = right_arm = NULL;
}

void Player::touched(Object * touched_by){
    const vector<ObjectAction> &actions = touched_by->getObjectActions();
    for(ObjectAction action : actions){
        if(action.getAction() == ObjectAction::TypeOfAction::FLOOR_TOUCHED)
            jump_moves = 0;
        else if(action.getAction() == ObjectAction::TypeOfAction::BODY_PART && action.getData() == "torso")
            setTorso(touched_by);
    }
}

void Player::setPhysics(b2World * world, float pos_x, float pos_y, float width, float height, bool dynamic){
}

void Player::moveRight(){
    //if(rotation.y < 90)
    //rotation.y+=2;
    b2Vec2 vel = head->getBody()->GetLinearVelocity();
    float force = 0;
    if(vel.x < 60){
        force =getMass()*300;
    }
    head->getBody()->ApplyForce(b2Vec2(force,0),head->getBody()->GetWorldCenter(), true);

}

void Player::moveLeft(){
    //if(rotation.y > -90)
    //rotation.y-=2;
    b2Vec2 vel = head->getBody()->GetLinearVelocity();
    float force = 0;
    if(vel.x > -60){
        force = -(getMass()*300);
    }
    head->getBody()->ApplyForce(b2Vec2(force,0),head->getBody()->GetWorldCenter(), true);
}

float Player::getMass(){
    float mass = 0;
    if(head) mass += head->getBody()->GetMass();

    return mass;
}

void Player::jump(){

    if(jump_moves < JUMPING_MAX){
        float force = getMass() * 20; //f = mv/t
        head->getBody()->ApplyLinearImpulse(b2Vec2(0,force), head->getBody()->GetWorldCenter(),true);
        jump_moves++;
    }
}


void Player::setLeftLeg(Object *object){
    if(left_leg!= NULL){
        delete left_leg;
    }
}
void Player::setRightLeg(Object *object){
    if(right_leg!= NULL){
        delete right_leg;
    }
}

void Player::draw(){
    if(head) head->draw();
    if(torso) torso->draw();
    if(left_leg) left_leg->draw(); 
    if(right_leg) right_leg->draw(); 
    if(left_arm) left_arm->draw(); 
    if(right_arm) right_arm->draw();

}

void Player::setPV(const glm::mat4 &PV) {
    if(head) head->setPV(PV);
    if(torso) torso->setPV(PV);
}

void Player::setTorso(Object *object){
    if(torso!= NULL){
        torso->destroy();
    }
    torso = object;

    torso->setObjectTouchListener(this);
    const b2Vec2 &position = torso->getBody()->GetPosition();

    world_action_provider->addWorldAction(new WorldAction(WorldAction::Action::CREATE_JOINT,
        head,new WorldAction::Position(b2Vec2(position.x + 1.5,position.y + torso->getHeight()+1.5),0),
        torso,new WorldAction::Position(b2Vec2(position.x,position.y+0.5),0)));
}

void Player::setHead(Object *object){
    if(head != NULL){
        delete head;
    }
    head = object;
    head->setObjectTouchListener(this);
    this->head->getBody()->SetBullet(true);
}
void Player::setLeftArm(Object *object){
    if(left_arm!= NULL){
        delete left_arm;
    }
}
void Player::setRightArm(Object *object){
    if(right_arm!= NULL){
        delete right_arm;
    }
}
Player::~Player(void)
{
}
