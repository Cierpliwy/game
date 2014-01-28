#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "Program.h"
#include "RenderTarget.h"
#include "Font.h"
#include <Box2D/Box2D.h>
#include "World.h"

class Game
{
public:
    Game() : 
        m_window(NULL), m_context(NULL), m_exit(false), 
        m_mapTarget(Map::MAP | Map::SPRITES | Map::BACKGROUND)
    {}
    ~Game();
    void initialize();
    void run();
    void cleanup();

private:
    		
    SDL_Window *m_window;
    SDL_GLContext m_context;
    bool m_exit;

    unsigned int m_mapTarget;

    World *world;
    Program objProgram;
    VertexShader objVertex;
    FragmentShader objFragment;
    GLuint whiteLocation;
    GLuint playerPosLocation;
    GLuint shadowLocation;

    Program shadowProgram;
    VertexShader shadowVertex;
    FragmentShader shadowFragment;
    GLuint shadowResolutionLocation;
    GLuint textureLocation;
    GLuint positionLocation;
    GLuint sizeLocation; 
    
    RenderTarget renderTarget;
    RenderTarget shadowTarget;
    Sprite viewportSprite;

    Font font;
};

#endif //GAME_H
