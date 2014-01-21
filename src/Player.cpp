#include "Player.h"


Player::Player(){
    human_movability = false;
    jump_moves = 0; 
    lives = 100;
    mHasFullBody = false;
    left_leg = right_leg = torso =head =left_arm = right_arm = NULL;
}

void Player::touched(Object * touched_by){
    vector<ObjectAction> &actions = touched_by->getObjectActions();

    vector<ObjectAction>::iterator &it = actions.begin();
    while(it != actions.end()){
        if(it->getAction() == ObjectAction::TypeOfAction::FLOOR_TOUCHED){
            jump_moves = 0;

        }else if(it->getAction() == ObjectAction::TypeOfAction::BODY_PART && it->getData() == "torso"){
            setTorso(touched_by);
            if(it->isSingleAction()){
                it = actions.erase(it);
                continue;
            }
        }
        else if(it->getAction() == ObjectAction::TypeOfAction::BODY_PART && it->getData() == "left_leg"){
            setLeftLeg(touched_by);
            if(it->isSingleAction()){
                it = actions.erase(it);
                continue;
            }
        }

        ++it;
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
    if(!torso){
        return;
    }

    if(left_leg!= NULL){
        delete left_leg;
    }

    left_leg = object;
    left_leg->setObjectTouchListener(this);

    const b2Vec2 &torso_position = torso->getBody()->GetPosition();
    b2Vec2 left_leg_pos(torso_position.x - (torso->getWidth())/2,torso_position.y - (torso->getHeight()/2));
    b2Vec2 torso_pos(torso_position.x,torso_position.y);
    b2RevoluteJointDef joint;
    joint.collideConnected = false;
    joint.bodyA = left_leg->getBody();
    joint.bodyB = torso->getBody();
    joint.localAnchorA.Set(0,(left_leg->getHeight())/2);
    joint.localAnchorB.Set(-(torso->getWidth())/2,-(torso->getHeight())/2);

    joint.referenceAngle = 0;
    joint.enableLimit = true;
    joint.lowerAngle = -5 *glm::pi<float>()/180;
    joint.upperAngle =  5 *glm::pi<float>()/180;

    world_action_provider->addWorldAction(new WorldAction(WorldAction::Action::CREATE_JOINT,
        left_leg,new WorldAction::Position(left_leg_pos,0),
        torso,new WorldAction::Position(torso_pos,0),joint));

    if(isBodyComplete()){
        setHumanMovability(true);
        return;
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
    const b2Vec2 &torso_position = torso->getBody()->GetPosition();
    b2Vec2 head_pos(torso_position.x,torso_position.y + torso->getHeight());
    b2Vec2 torso_pos(torso_position.x,torso_position.y);
    b2RevoluteJointDef joint;
    joint.collideConnected = false;
    joint.bodyA = head->getBody();
    joint.bodyB = torso->getBody();
    joint.localAnchorA.Set(0,-(head->getHeight())/2);
    joint.localAnchorB.Set(0,(torso->getHeight())/2);

    joint.referenceAngle = 0;
    joint.enableLimit = true;
    joint.lowerAngle = -15 *glm::pi<float>()/180;
    joint.upperAngle =  15 *glm::pi<float>()/180;

    world_action_provider->addWorldAction(new WorldAction(WorldAction::Action::CREATE_JOINT,
        head,new WorldAction::Position(head_pos,0),
        torso,new WorldAction::Position(torso_pos,0),joint));
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

bool Player::isBodyComplete(){
    if(!head)
        return false;
    if(!torso)
        return false;
    if(!left_leg)
        return false;
    if(!right_leg)
        return false;
    if(!left_arm)
        return false;
    if(!right_arm)
        return false;

    return true;
}
Player::~Player(void)
{
}
