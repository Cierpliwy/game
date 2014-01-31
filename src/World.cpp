#include "World.h"


World::World(void): world(NULL), player(NULL), particles(NULL),m_map()
{

}

void World::initializeWorldPhysics(){
    if(world != NULL){
        delete world;
    }
    world = new b2World(b2Vec2(0.0f, 10*-9.81f));
    world->SetAllowSleeping(true);    
    world->SetContinuousPhysics(true);
    world->SetContactListener(this); 
}

void World::initWorld(string map_path, Program *program, float width, float depth, float uvFix){

    this->program = program;
    initializeWorldPhysics();
    // initialize objects below this place
    // Load fire map by default
    m_map.init();
    m_map.load(map_path.c_str());
    m_map.generate(width, depth, uvFix); //setPhysics after this !!!!
    m_map.setPhysics(world);

    if(player != NULL){
        delete player;
    }

    player = new Player();
    player->setWorldActionProvider(this);
}

void World::addObject(string mesh_path, float pos_x, float pos_y, float width, float height, vector<ObjectAction> actions ,bool dynamic){
    Object *object = new Object();
    object->setWorldActionProvider(this);
    object->loadMesh(mesh_path.c_str());
    object->setPhysics(world, pos_x, pos_y, width, height);
    object->setProgram(*program);
    object->setObjectActions(actions);
    objects.push_back(object);
}

void World::setParticles(string mesh_path, float width, float height){
    if(particles){
        delete particles;
    }
    particles = new Particles();
    particles->setProgram(*program);
    particles->loadMesh(mesh_path.c_str());
    particles->setPhysics(world,0,10,width,height);
}

void World::step(float step){

    while(world_actions.size()){
        if(world_actions.back()->action == WorldAction::Action::CREATE_JOINT){
            static_cast<Object*>(world_actions.back()->obj1)->getBody()->SetTransform(world_actions.back()->pos1->position,world_actions.back()->pos1->angle);
            static_cast<Object*>(world_actions.back()->obj2)->getBody()->SetTransform(world_actions.back()->pos2->position,world_actions.back()->pos2->angle);
            world->CreateJoint(&world_actions.back()->revoluteJointDef);
        } else if(world_actions.back()->action == WorldAction::Action::SET_BODY_POSITION){
            static_cast<Object*>(world_actions.back()->obj1)->getBody()->SetTransform(world_actions.back()->pos1->position,world_actions.back()->pos1->angle);
        }
        world_actions.pop_back();
    }

    time_step = step;
    world->Step(step,4,4);

    player->setTimeInterval(step);

    b2Body* body = world->GetBodyList();
    b2Body* body_tmp;
    while(body != NULL){
        Object *tmp = static_cast<Object*>(body->GetUserData());
        body_tmp = body->GetNext();
        if(true == tmp->isFlagedForDelete()){
            world->DestroyBody(body);
            //delete tmp;
        }
        body = body_tmp;
    }
}


void World::setPlayerHead(string mesh_path, float pos_x, float pos_y, float width, float height){
    Object *head = new Object();
    head->setProgram(*program); 
    head->loadMesh(mesh_path.c_str());
    head->setPhysics(world,pos_x, pos_y, width,height);

    player->setHead(head);
}


void World::BeginContact(b2Contact * contact){
    Object *object1 = (Object*)contact->GetFixtureA()->GetUserData();
    Object *object2 = (Object*)contact->GetFixtureB()->GetUserData();

    object1->touched(object2);
    object2->touched(object1);
}

void World::EndContact(b2Contact * contact){
}	

void World::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse){
}	

void World::PreSolve(b2Contact * contact, const b2Manifold * oldManifold){
    
    string name1 = static_cast<Object *>(contact->GetFixtureA()->GetBody()->GetUserData())->getObjectName();
    string name2 = static_cast<Object *>(contact->GetFixtureB()->GetBody()->GetUserData())->getObjectName();

    if(name1 == "particle" || name1 == "ceiling"){
        if(name2 == "particle" || name2 == "ceiling"){
        
            contact->SetEnabled(false);
        }
    }
}

World::~World(void)
{
    if(world != NULL){
        delete world;
    }

    if(particles){
        delete particles;
    }

    if(player){
        delete player;
    }
}
