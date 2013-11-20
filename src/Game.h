#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "Program.h"
#include "Map.h"

class Game
{
public:
    Game() : m_window(NULL), m_context(NULL), m_exit(false) {}

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

    Map m_map;
    GLuint m_MVPLocation;
    GLuint m_lightLocation;
    GLuint m_lightSizeLocation;
};

#endif //GAME_H
