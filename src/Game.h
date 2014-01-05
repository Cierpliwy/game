#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "Program.h"
#include "Map.h"
#include "Object.h"
#include "Player.h"
#include "RenderTarget.h"
#include "Font.h"
#include <Box2D/Box2D.h>


class Game : public b2ContactListener 
{
public:
    Game() : 
        m_window(NULL), m_context(NULL), m_exit(false), world(NULL), 
        m_mapTarget(Map::MAP | Map::SPRITES | Map::BACKGROUND)
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

    b2World *world;
    Player *player;
    Map m_map;
    unsigned int m_mapTarget;

    Program objProgram;
    VertexShader objVertex;
    FragmentShader objFragment;
    GLuint whiteLocation;

    Program shadowProgram;
    VertexShader shadowVertex;
    FragmentShader shadowFragment;
    GLuint shadowResolutionLocation;
    GLuint textureLocation;
    GLuint positionLocation;
    GLuint sizeLocation;

    Object object;
    RenderTarget renderTarget;
    RenderTarget shadowTarget;
    Sprite viewportSprite;

    Font font;
};

#endif //GAME_H
