#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "Program.h"
#include "Map.h"
#include "Object.h"

class Game
{
public:
    Game() : 
        m_window(NULL), m_context(NULL), m_exit(false),
        m_mapTarget(Map::MAP | Map::SPRITES | Map::GRID),
        object("../data/cube.obj") {}

    void initialize();
    void run();
    void cleanup();

private:
    SDL_Window *m_window;
    SDL_GLContext m_context;
    bool m_exit;

    VertexShader m_vertex;
    FragmentShader m_fragment;
    Program m_program;
    
    VertexShader m_simpleVertex;
    FragmentShader m_simpleFragment;
    Program m_simpleProgram;

    Map m_map;
    unsigned int m_mapTarget;

    Object object;
    GLuint m_MVPLocation;
    GLuint m_lightLocation;
    GLuint m_lightSizeLocation;
    GLuint m_textureLocation;
};

#endif //GAME_H
