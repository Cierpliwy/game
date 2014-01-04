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

using namespace glm;

void Game::initialize() {

    // Initialize SDL library
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        throw GameException(GameException::SDL, "Init");

    // Initialize SDL_image library
    int flags = IMG_Init(IMG_INIT_PNG);
    if ((flags & IMG_INIT_PNG) != flags) 
        throw GameException(GameException::SDL_IMAGE, "Init");

    // Use OpenGL version 3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Enable double buffering and set depth accuracy to 24 bits
    // for depth buffer.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create SDL window
    m_window = SDL_CreateWindow("Gra", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 800, 600, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );//| SDL_WINDOW_FULLSCREEN);
    if (!m_window) 
        throw GameException(GameException::SDL, "Window");

    // Create OpenGL context for window.
    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context) 
        throw GameException(GameException::SDL, "Context");

    // Enable v-sync
    SDL_GL_SetSwapInterval(1);

    // Load OpenGL by getting functions bodies (for Windows)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) 
        throw GameException(GameException::GLEW, "Init", err);

    // Common OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);

    initializeWorldPhysics();
    // initialize objects below this place
    // Load fire map by default
    m_map.init();
    m_map.load("../data/maps/fire/map");
    m_map.generate(200.0f, -40.0f, 0.1f); //setPhysics after this !!!!
    m_map.setPhysics(world);

    // Object shader
    objVertex.load("../data/objVertex.glsl");
    objVertex.compile();
    objFragment.load("../data/objFragment.glsl");
    objFragment.compile();
    objProgram.create();
    objProgram.setFragmentShader(objFragment);
    objProgram.setVertexShader(objVertex);
    objProgram.link();

    // Shadow shader
    shadowVertex.load("../data/shadowCastVertex.glsl");
    shadowVertex.compile();
    shadowFragment.load("../data/shadowCastFragment.glsl");
    shadowFragment.compile();
    shadowProgram.create();
    shadowProgram.setFragmentShader(shadowFragment);
    shadowProgram.setVertexShader(shadowVertex);
    shadowProgram.link();
    textureLocation = shadowProgram.getUniformLocation("tex");
    shadowResolutionLocation = shadowProgram.getUniformLocation("res");
    positionLocation = shadowProgram.getUniformLocation("pos");
    sizeLocation = shadowProgram.getUniformLocation("size");

    // cube that has 1.8 meters height and width
    object.loadMesh("../data/body.obj");
    object.setPhysics(world, 15, 40, 5.8, 5.8);
    object.setProgram(objProgram);

    player = new Player();
    player->setProgram(objProgram);
    player->loadMesh("../data/head.obj");
    player->setPhysics(world,20,40,5.5,5.5);

    //Set render target
    renderTarget.create(GL_TEXTURE_2D, 4096, 2048);
    viewportSprite.generate(Rect<vec3>(
                            vec3(-1.0f, -1.0f, 0.0f),
                            vec3(1.0f, -1.0f, 0.0f),
                            vec3(1.0f, 1.0f, 0.0f),
                            vec3(-1.0f, 1.0f, 0.0f)));
    shadowTarget.create(GL_TEXTURE_1D, 4096, 1);
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
    world = new b2World(b2Vec2(0.0f, 10*-9.81f));
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
    // Ticks used for high accuracy clock
    Uint64 ticks = SDL_GetPerformanceCounter();
    // Time from the beginning of a program in seconds
    float time = 0;
    Uint64 timeStart = ticks;
    // Last frame time in seconds
    float delta = 0;
    // Player position
    vec2 pos(3.5f, 2.0f);

    //tmp
    float offset = time;

    while (!m_exit) {

        if (glGetError() != GL_NO_ERROR) cout << "ERROR!" << endl;

        // Update time data
        delta = static_cast<float>(SDL_GetPerformanceCounter() - ticks) /
            SDL_GetPerformanceFrequency();
        ticks = SDL_GetPerformanceCounter();
        time = static_cast<float>(SDL_GetPerformanceCounter() - timeStart) /
            SDL_GetPerformanceFrequency();

        // Handle input events
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
                m_map.load("../data/maps/cold/map");
                m_map.generate(200.0f, -40.0f, 0.1f); //setPhysics after this !!!!
                m_map.setPhysics(world);
                offset = time;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
                m_map.load("../data/maps/fire/map");
                m_map.generate(200.0f, -40.0f, 0.1f); //setPhysics after this !!!!
                m_map.setPhysics(world);
                offset = time;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o) {
                offset = time;
            }
        }

        // Update position 
        const Uint8 *keymap = SDL_GetKeyboardState(NULL);
        if (keymap[SDL_SCANCODE_W]) player->jump();
        if (keymap[SDL_SCANCODE_A]) player->moveLeft();
        if (keymap[SDL_SCANCODE_D]) player->moveRight();

        // Get window ratio
        int w,h;
        SDL_GetWindowSize(m_window, &w, &h);
        float ratio = static_cast<float>(w)/h;

        // Basic matrix settings
        pos.x = player->getPosition().x;
        pos.y = player->getPosition().y; 
        glm::mat4 projection = glm::perspective(45.0f, ratio, 0.1f,1000.0f);
        glm::mat4 view = glm::lookAt(vec3(pos, 125.0f), vec3(pos, 0.0f), vec3(0,1,0));
        glm::mat4 PV = projection * view;

        glm::mat4 mapProjection = glm::ortho(0.0f,m_map.getWidth(),
                                             0.0f,m_map.getHeight(),
                                             -1.0f,1.0f);
        glm::mat4 PV2 = mapProjection;

        // Use this matrix for setting model's position in a world
        glm::mat4 model(1.0f); 

        // Set clear color
        glClearColor(0,0,0,0);

        // Draw shadow casting mesh to texture
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.getFramebuffer());
        glViewport(0,0,4096,2048);
        glClear(GL_COLOR_BUFFER_BIT); 
        m_map.setPV(PV2);
        m_map.setVisibility((time-offset));
        m_map.draw(Map::MAP | Map::WHITE);
        object.setPV(PV2);
        object.setRotation(vec3(time*180));
        object.draw(&renderTarget.getTexture());

        // Get shadow data
        glBindFramebuffer(GL_FRAMEBUFFER, shadowTarget.getFramebuffer());
        glViewport(0,0,4096,1);
        glClear(GL_COLOR_BUFFER_BIT); 
        shadowProgram.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTarget.getTexture().id());
        glUniform1i(textureLocation, 0);
        glUniform1f(shadowResolutionLocation, 0.1);
        glUniform2f(positionLocation, pos.x, pos.y);
        glUniform2f(sizeLocation, m_map.getWidth(), m_map.getHeight());
        viewportSprite.draw();

        // Final render
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,800,600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_1D, shadowTarget.getTexture().id());
        glGenerateMipmap(GL_TEXTURE_1D);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_map.setPlayerPos(pos);
        m_map.setShadowTexture(&shadowTarget.getTexture());
        m_map.setPV(PV);
        m_map.setBackgroundX(pos.x/m_map.getWidth());
        m_map.setBackgroundY(pos.y/m_map.getHeight());
        m_map.draw(m_mapTarget);
        glBindTexture(GL_TEXTURE_1D, shadowTarget.getTexture().id());
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Draw object
        object.setPV(PV);
        object.setRotation(vec3(0,time*180,0));
        object.draw();

        player->setPV(PV);
        player->setRotation(vec3(0,time*180,0));
        player->draw();

        // Flip buffers
        SDL_GL_SwapWindow(m_window);
        world->Step(delta,4,4);
    }
}


void Game::BeginContact(b2Contact * contact){
    Object *object1 = (Object*)contact->GetFixtureA()->GetUserData();
    Object *object2 = (Object*)contact->GetFixtureB()->GetUserData(); 
    object1->touched(object2);
    object2->touched(object1);
}

void Game::EndContact(b2Contact * contact){
}	

void Game::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse){
}	

void Game::PreSolve(b2Contact * contact, const b2Manifold * oldManifold){
}
