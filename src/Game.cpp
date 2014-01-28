#include "Debug.h"
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

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
#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context) 
        throw GameException(GameException::SDL, "Context");

    // Enable v-sync
    SDL_GL_SetSwapInterval(0);

    // Load OpenGL by getting functions bodies (for Windows)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) 
        throw GameException(GameException::GLEW, "Init", err);

#ifdef DEBUG
    // If we have debug output extension. We would like to use it ;)
    if (GLEW_AMD_debug_output) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallbackAMD((GLDEBUGPROCAMD)gameDebugCallbackAMD, nullptr);
    } else if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallbackARB((GLDEBUGPROCARB)gameDebugCallbackARB, nullptr);
    }
#endif

    // Common OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);


    // Object shader
    objVertex.load("../data/objVertex.glsl");
    objVertex.compile();
    objFragment.load("../data/objFragment.glsl");
    objFragment.compile();
    objProgram.create();
    objProgram.setFragmentShader(objFragment);
    objProgram.setVertexShader(objVertex);
    objProgram.link();
    whiteLocation = objProgram.getUniformLocation("white");
    shadowLocation = objProgram.getUniformLocation("shadow");
    playerPosLocation = objProgram.getUniformLocation("playerPos");

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

    world = new World();
    world->initWorld("../data/maps/fire/map", &objProgram, 200.0f, -40.0f, 0.1f);

    vector<ObjectAction> actions;
    actions.push_back(ObjectAction(ObjectAction::TypeOfAction::BODY_PART,"torso",true));
    world->addObject("../data/body.obj",15.0f, 40.0f, 4.8f, 4.8f,actions);

    vector<ObjectAction> left_leg_actions;
    left_leg_actions.push_back(ObjectAction(ObjectAction::TypeOfAction::BODY_PART, "left_leg", true));
    world->addObject("../data/foot.obj", 128.0, 67.5, 2.0f, 2.0f, left_leg_actions);

    vector<ObjectAction> right_leg_actions;
    right_leg_actions.push_back(ObjectAction(ObjectAction::TypeOfAction::BODY_PART, "right_leg", true));
    world->addObject("../data/foot.obj", 111.0, 28.0, 2.0f, 2.0f, right_leg_actions);

    vector<ObjectAction> right_arm_actions;
    right_arm_actions.push_back(ObjectAction(ObjectAction::TypeOfAction::BODY_PART, "right_arm", true));
    world->addObject("../data/hand.obj", 69.0, 14.0, 2.0f, 2.0f, right_arm_actions);

    vector<ObjectAction> left_arm_actions;
    left_arm_actions.push_back(ObjectAction(ObjectAction::TypeOfAction::BODY_PART, "left_arm", true));
    world->addObject("../data/hand.obj", 161.0, 22.0, 2.0f, 2.0f, left_arm_actions);

    world->setPlayerHead("../data/head.obj",20,20,4.5,4.5);
    world->setParticles("../data/snow.obj",200,100);



    //Set render target
    renderTarget.create(GL_TEXTURE_2D, 4096, 2048);
    viewportSprite.generate(Rect<vec3>(
        vec3(-1.0f, -1.0f, 0.0f),
        vec3(1.0f, -1.0f, 0.0f),
        vec3(1.0f, 1.0f, 0.0f),
        vec3(-1.0f, 1.0f, 0.0f)));
    shadowTarget.create(GL_TEXTURE_1D, 4096, 1);

    //Load font
    font.load("../data/font.png");
}

Game::~Game(){
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
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
#ifdef _WIN32
            OutputDebugString((const char*)gluErrorString(error));
            OutputDebugString("\n");
#endif
            cout << "ERROR!" << endl;
        }

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
                world->initWorld("../data/maps/cold/map", &objProgram, 200.0f, -40.0f, 0.1f);
                offset = time;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
                world->initWorld("../data/maps/fire/map", &objProgram, 200.0f, -40.0f, 0.1f);

                offset = time;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o) {
                offset = time;
            }
        }

        // Update position 
        const Uint8 *keymap = SDL_GetKeyboardState(NULL);
        if (keymap[SDL_SCANCODE_W]) world->playerJump();
        if (keymap[SDL_SCANCODE_A]) world->playerMoveLeft();
        if (keymap[SDL_SCANCODE_D]) world->playerMoveRight();

        // Get window ratio
        int w,h;
        SDL_GetWindowSize(m_window, &w, &h);
        float ratio = static_cast<float>(w)/h;

        // Basic matrix settings
        const b2Vec2 player_position = world->getPlayerPosition();
        pos.x = player_position.x;
        pos.y = player_position.y; 
        pos.y += 2; //Move eye up.
        glm::mat4 projection = glm::perspective(45.0f, ratio, 0.1f,1000.0f);
        glm::mat4 view = glm::lookAt(vec3(pos, 125.0f), vec3(pos, 0.0f), vec3(0,1,0));
        glm::mat4 PV = projection * view;

        glm::mat4 mapProjection = glm::ortho(0.0f,world->getMap()->getWidth(),
            0.0f,world->getMap()->getHeight(),
            -1.0f,1.0f);
        glm::mat4 PV2 = mapProjection;

        // Use this matrix for setting model's position in a world
        glm::mat4 model(1.0f); 

        // Set clear color
        glClearColor(0,0,0,0);

        // Draw shadow casting mesh to texture
        Uint64 shadowMapTime = SDL_GetPerformanceCounter();
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.getFramebuffer());
        glViewport(0,0,4096,2048);
        glClear(GL_COLOR_BUFFER_BIT); 
        world->getMap()->setPV(PV2);
        world->getMap()->setVisibility((time-offset));
        world->getMap()->draw(Map::MAP | Map::WHITE);

        objProgram.use();
        glUniform1i(whiteLocation, 1);
        for(Object *object: *(world->getObjects())){
            if (object->getCastShadow()) {
                object->setPV(PV2);
                object->draw();
            }
        }

        float shadowMapMs = 
            static_cast<float>(SDL_GetPerformanceCounter()-shadowMapTime) /
            SDL_GetPerformanceFrequency() * 1000.0f;

        // Get shadow data
        Uint64 rayTime = SDL_GetPerformanceCounter();
        glBindFramebuffer(GL_FRAMEBUFFER, shadowTarget.getFramebuffer());
        glViewport(0,0,4096,1);
        glClear(GL_COLOR_BUFFER_BIT); 
        shadowProgram.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTarget.getTexture().id());
        glUniform1i(textureLocation, 0);
        glUniform1f(shadowResolutionLocation, 0.1f);
        glUniform2f(positionLocation, pos.x, pos.y);
        glUniform2f(sizeLocation, world->getMap()->getWidth(), world->getMap()->getHeight());
        viewportSprite.draw();
        float rayMs = 
            static_cast<float>(SDL_GetPerformanceCounter()-rayTime) /
            SDL_GetPerformanceFrequency() * 1000.0f;

        // Final render
        Uint64 renderTime = SDL_GetPerformanceCounter();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,800,600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_1D, shadowTarget.getTexture().id());
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_1D, 0);

        world->getMap()->setPlayerPos(pos);
        world->getMap()->setShadowTexture(&shadowTarget.getTexture());
        world->getMap()->setPV(PV);
        world->getMap()->setBackgroundX(pos.x/world->getMap()->getWidth());
        world->getMap()->setBackgroundY(pos.y/world->getMap()->getHeight());
        world->getMap()->draw(m_mapTarget);

        glBindTexture(GL_TEXTURE_1D, shadowTarget.getTexture().id());
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_1D, 0);

        // Draw object
        objProgram.use();
        glUniform1i(whiteLocation, 0);
        glUniform2f(playerPosLocation, pos.x, pos.y);
        glUniform1i(shadowLocation, 2);
        for(Object *object: *(world->getObjects())){
            object->setPV(PV);
            object->draw();
        }


        //draw snow
        if(world->hasParticles()){
            world->getParticles()->setPV(PV);
            world->getParticles()->draw();
        }

        world->getPlayer()->setPV(PV);
        world->getPlayer()->draw();
        float renderMs = 
            static_cast<float>(SDL_GetPerformanceCounter()-renderTime) /
            SDL_GetPerformanceFrequency() * 1000.0f;

        // Print some text
        font.print("Twoja pozycja:\n\nx:\ny:",0.03,-0.9,0.9);
        font.print(to_string(pos.x),0.03,-0.8,0.9-0.06*1.5);
        font.print(to_string(pos.y),0.03,-0.8,0.9-0.09*1.5);
        font.print(to_string(1/delta),0.03,0.6,0.9);
        font.print("FPS", 0.03, 0.9,0.9);


        font.print("Shadow map:",0.03,-0.9,0.6);
        font.print(to_string(shadowMapMs),0.03,-0.5,0.6);

        font.print("Ray map:",0.03,-0.9,0.55);
        font.print(to_string(rayMs),0.03,-0.5,0.55);

        font.print("Final render:",0.03,-0.9,0.50);
        font.print(to_string(renderMs),0.03,-0.5,0.50);

        // Flip buffers
        SDL_GL_SwapWindow(m_window);

        world->step(delta);
    }
}
