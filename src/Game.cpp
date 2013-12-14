
#ifdef WIN32
#include "Windows.h"
#include "WinBase.h"
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Game.h"
#include "GameException.h"

using namespace std;
using namespace glm;

void Game::initialize() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        throw GameException(GameException::SDL, "Init");

    int flags = IMG_Init(IMG_INIT_PNG);
    if ((flags&IMG_INIT_PNG) != flags) 
        throw GameException(GameException::SDL_IMAGE, "Init");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow("Gra", SDL_WINDOWPOS_CENTERED, 
                                SDL_WINDOWPOS_CENTERED, 1200, 675, 
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!m_window) 
        throw GameException(GameException::SDL, "Window");

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context) 
        throw GameException(GameException::SDL, "Context");

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) 
        throw GameException(GameException::GLEW, "Init", err);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    m_vertex.load("../data/vertex.glsl");
    m_vertex.compile();
    m_fragment.load("../data/fragment.glsl");
    m_fragment.compile();
    m_program.create();
    m_program.setFragmentShader(m_fragment);
    m_program.setVertexShader(m_vertex);
    m_program.link();
    m_program.use();

    m_MVPLocation = m_program.getUniformLocation("MVP");
    m_lightLocation = m_program.getUniformLocation("light");
    m_lightSizeLocation = m_program.getUniformLocation("lightSize");
    m_textureLocation = m_program.getUniformLocation("texSampler");

    m_map.load("../data/map");
    m_map.generate(5.0f, -1.0f);
    object.loadObject();
}

void Game::cleanup() {
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
 
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    Uint64 time = SDL_GetPerformanceCounter();
    float delta = 0;
    vec2 pos(0.0f, 0.0f);

    while (!m_exit) {
        delta = static_cast<float>(SDL_GetPerformanceCounter() - time) /
                                   SDL_GetPerformanceFrequency();
        time = SDL_GetPerformanceCounter();
        
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) 
                m_exit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                m_exit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_KP_ENTER)
                SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
        }

        const Uint8 *keymap = SDL_GetKeyboardState(NULL);
        if (keymap[SDL_SCANCODE_RIGHT]) pos.x += delta;
        if (keymap[SDL_SCANCODE_LEFT]) pos.x -= delta;
        if (keymap[SDL_SCANCODE_UP]) pos.y += delta;
        if (keymap[SDL_SCANCODE_DOWN]) pos.y -= delta;

        int w,h;
        SDL_GetWindowSize(m_window, &w, &h);
        float ratio = static_cast<float>(w)/h;

        glm::mat4 projection = glm::perspective(45.0f, ratio, 0.1f, 100.0f);

        glm::mat4 view = glm::lookAt(vec3(pos, 5.0f), vec3(pos, 0.0f), vec3(0,1,0));
        glm::mat4 MVP = projection * view;

        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        glUniform2f(m_lightLocation, pos.x, pos.y);
        glUniform1f(m_lightSizeLocation, delta * 50.0f);

        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_map.draw(m_textureLocation);
     
        //Rotate for fun. Look at light! Why is it happening? :D
        MVP = glm::rotate(MVP, 45.0f, glm::vec3(1.0f,1.0f,1.0f));
        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        object.draw(m_textureLocation);

        SDL_GL_SwapWindow(m_window);
    }
}
