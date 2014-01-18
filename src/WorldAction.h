#pragma once

//structs that are need to provide actions to our world

struct WorldAction{
    enum Action{
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

    WorldAction(Action _action, void* _obj1, Position *_pos1 ,void * _obj2 = NULL, Position *_pos2 = NULL): action(_action),
     obj1(_obj1), obj2(_obj2), pos1(_pos1), pos2(_pos2){
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
