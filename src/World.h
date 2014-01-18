#pragma once

#include <vector>
#include "Object.h"
#include "Player.h"
#include "Particles.h"
#include "Map.h"
#include "WorldAction.h"


class World : public b2ContactListener , public WorldActionProvider 
{
    vector<WorldAction *> world_actions;
    vector<Object *> objects;
    b2World *world;
    Player *player;
    Particles *particles;
    Map m_map;
    Program *program;

    float time_step;

    void initializeWorldPhysics();
public:
    World(void);
    void initWorld(string map_path, Program *program, float width, float depth, float uvFix);
    void addObject(string mesh_path, float pos_x, float pos_y, float width, float height, vector<ObjectAction> actions ,bool dynamic = true);
    void setParticles(string mesh_path, float width, float height);
    void setPlayerHead(string mesh_path, float pos_x, float pos_y, float width, float height);

    //processing all thing that should changes in b2world
    void step(float step);

    const b2Vec2 getPlayerPosition(){return player->getPosition();}

    void playerJump(){player->jump();}
    void playerMoveLeft(){player->moveLeft();}
    void playerMoveRight(){player->moveRight();}

    vector<Object*> * getObjects(){return &objects;}
    Particles * getParticles(){return particles;}
    Map* getMap(){return &m_map;}
    Player* getPlayer(){return player;}
    bool hasParticles(){return particles!=NULL;}

    void addWorldAction(WorldAction *action){world_actions.push_back(action);}

    //overloaded functions from b2ContactListener
    void BeginContact(b2Contact * contact);
    void EndContact(b2Contact * contact);	
    void PostSolve(b2Contact * contact, const b2ContactImpulse * impulse);	
    void PreSolve(b2Contact * contact, const b2Manifold * oldManifold);

    ~World(void);
};

