#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "Program.h"
#include "Map.h"
#include "Object.h"
#include "Player.h"
#include <Box2D/Box2D.h>


class Game : public b2ContactListener 
{
public:
    Game() : 
        m_window(NULL), m_context(NULL), world(NULL), m_exit(false),
        m_mapTarget(Map::MAP | Map::SPRITES | Map::GRID)
    {}
    ~Game();
    void initialize();
    void run();
    void cleanup();

private:
    void initializeWorldPhysics();

    //overloaded functions from b2ContactListener
    void BeginContact(b2Contact * contact);
    void EndContact(b2Contact * contact);	
    void PostSolve(b2Contact * contact, const b2ContactImpulse * impulse);	
    void PreSolve(b2Contact * contact, const b2Manifold * oldManifold);
    		
    SDL_Window *m_window;
    SDL_GLContext m_context;
    bool m_exit;

    VertexShader m_vertex;
    FragmentShader m_fragment;
    Program m_program;

    VertexShader m_simpleVertex;
    FragmentShader m_simpleFragment;
    Program m_simpleProgram;

    b2World *world;
    Player *player;
    Map m_map;
    unsigned int m_mapTarget;

    Object object;
    GLuint m_MVPLocation;
    GLuint m_lightLocation;
    GLuint m_lightSizeLocation;
    GLuint m_textureLocation;
};

#endif //GAME_H
