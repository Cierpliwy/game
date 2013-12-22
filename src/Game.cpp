
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
    glFrontFace(GL_CW);

    m_vertex.load("../data/vertex.glsl");
    m_vertex.compile();
    m_fragment.load("../data/fragment.glsl");
    m_fragment.compile();
    m_program.create();
    m_program.setFragmentShader(m_fragment);
    m_program.setVertexShader(m_vertex);
    m_program.link();

    m_simpleVertex.load("../data/simpleVertex.glsl");
    m_simpleVertex.compile();
    m_simpleFragment.load("../data/simpleFragment.glsl");
    m_simpleFragment.compile();
    m_simpleProgram.create();
    m_simpleProgram.setFragmentShader(m_simpleFragment);
    m_simpleProgram.setVertexShader(m_simpleVertex);
    m_simpleProgram.link();

    m_MVPLocation = m_program.getUniformLocation("MVP");
    m_lightLocation = m_program.getUniformLocation("light");
    m_lightSizeLocation = m_program.getUniformLocation("lightSize");
    m_textureLocation = m_program.getUniformLocation("texSampler");

    m_map.load("../data/fire/map");
    m_map.generate(7.0f, -1.0f, 2.0f);

    initializeWorldPhysics();
    // initialize objects below this place

    object.loadMesh("../data/cube.obj");
    // cube that has 1.8 meters height and width
    object.setPhysics(world,10,20,1.8,1.8);
}

Game::~Game(){
    if(world != NULL){
        delete world;
    }
}
void Game::initializeWorldPhysics(){
    if(world != NULL){
        delete world;
    }
    world = new b2World(b2Vec2(0.0f, 9.81f));
    world->SetAllowSleeping(true);    
    world->SetContinuousPhysics(true);
    world->SetContactListener(this); 

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
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s)
                m_mapTarget ^= Map::SPRITES;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_g)
                m_mapTarget ^= Map::GRID;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_c) {
                m_map.load("../data/cold/map");
                m_map.generate(7.0f, -1.0f, 2.0f);
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
                m_map.load("../data/fire/map");
                m_map.generate(7.0f, -1.0f, 2.0f);
            }
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

        static float angle = 45.0f;

        m_program.use();
        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        glUniform2f(m_lightLocation, pos.x, pos.y);
        glUniform1f(m_lightSizeLocation, sin(angle*0.1f)*0.08f + 1.8f);

        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_map.draw(m_textureLocation, m_mapTarget & (~Map::GRID));

        // Draw GRID
        m_simpleProgram.use();
        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        if (m_mapTarget & Map::GRID)
            m_map.draw(m_textureLocation, Map::GRID);

        //Rotate for fun. Look at light! Why is it happening? :D
        m_program.use();
        //angle += delta * 45.0f;
        //MVP = glm::scale(MVP, glm::vec3(0.2f,0.2f,0.2f));
        //MVP = glm::rotate(MVP, angle, glm::vec3(1.0f,1.0f,1.0f));
        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        object.draw(m_textureLocation, MVP, m_MVPLocation);

        MVP = glm::translate(MVP,  glm::vec3(2.0f,0.0f,0.0f));
        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        object.draw(m_textureLocation, MVP, m_MVPLocation);


        SDL_GL_SwapWindow(m_window);

        world->Step(delta,4,4);
    }
}


void Game::BeginContact(b2Contact * contact){
}

void Game::EndContact(b2Contact * contact){
}	

void Game::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse){
}	

void Game::PreSolve(b2Contact * contact, const b2Manifold * oldManifold){
}