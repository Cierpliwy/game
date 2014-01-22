#pragma once

//structs that are need to provide actions to our world

struct WorldAction{
    enum Action{
        SET_BODY_POSITION,
        CREATE_JOINT,
        CREATE_BODY
    };

    struct Position{
        b2Vec2 position;
        float angle;
        Position(b2Vec2 _position, float _angle): position(_position), angle(_angle) {}
    };

    Action action;
    void *obj1, *obj2;
    Position *pos1, *pos2;
    b2RevoluteJointDef revoluteJointDef;

    WorldAction(Action _action, void* _obj1, Position *pos): obj1(_obj1), action(_action), pos1(pos){
    }

    WorldAction(Action _action, void* _obj1, Position *_pos1 ,void * _obj2 , Position *_pos2 , b2RevoluteJointDef joint): action(_action),
     obj1(_obj1), obj2(_obj2), pos1(_pos1), pos2(_pos2), revoluteJointDef(joint){
    }

    ~WorldAction(){
        if(pos1){
            delete pos1;
        }
        if(pos2){
            delete pos2;
        }
    }
};

struct WorldActionProvider{
    virtual void addWorldAction(WorldAction * action) = 0;
};
