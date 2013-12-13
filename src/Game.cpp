#include "Windows.h"
#include "WinBase.h"
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

    // Inicjalizujemy bibliotekę SDL, włączamy póki co tylko tryb VIDEO.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        throw GameException(GameException::SDL, "Init");

    // Inicjalizujemy bibliotekę wczytującą pliki graficzne z popularnych
    // formatów. SDL domyślnie obsługuje tylko BMP...
    int flags = IMG_Init(IMG_INIT_PNG);
    if ((flags&IMG_INIT_PNG) != flags) 
        throw GameException(GameException::SDL_IMAGE, "Init");

    // Przygotuwujemy okno do utworzenia kontekstu OpenGL
    // Prosimy o wersje 3.0 - pierwszą z funkcjonalnością, API
    // pozabwioną fixed pipeline.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Użymamy podwójnego buforowania i bufora głębi o rozdzielczości 24 bitów.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Tworzymy okno SDL i sugerujemy, że będzie one wykorzystywało
    // renderowanie w OpenGL.
	m_window = SDL_CreateWindow("Gra", SDL_WINDOWPOS_CENTERED, 
		                        SDL_WINDOWPOS_CENTERED, 1200, 675, 
								SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!m_window) 
        throw GameException(GameException::SDL, "Window");

    // Tworzymy kontekst OpenGL (konieczy do komunikacji) przywiązany do
    // danego okna.
	m_context = SDL_GL_CreateContext(m_window);
    if (!m_context) 
        throw GameException(GameException::SDL, "Context");

    // Upewniamy się, że VSync jest włączony. W celu testowania wydajności
    // FPS można tą wartość ustawić na 0.
	SDL_GL_SetSwapInterval(1);

    // Pobieramy wskaźniki do funkcji z API OpenGL. Wymagane to jest by
    // gra działała także na Windows, który olewa dostarczanie nowszych 
    // bibliotek niż 1.1.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) 
        throw GameException(GameException::GLEW, "Init", err);

    // Wczytujemy vertex shader i fragment shader. Kolejno odpowiedzialnych
    // za to jak wyświetlane zostaną wierzchołki i piksele. Ostatecznie łączymy
    // je do jednego programu, na który się składają powyższe shadery.
    m_vertex.load("../data/vertex.glsl");
    m_vertex.compile();
    m_fragment.load("../data/fragment.glsl");
    m_fragment.compile();
    m_program.create();
    m_program.setFragmentShader(m_fragment);
    m_program.setVertexShader(m_vertex);
    m_program.link();
    m_program.use();

    // Pobieramy lokację zmiennych typu "uniform" tak aby móc przesylać
    // z programu zmienne do karty graficznej wpływającej na proces 
    // renderowania. Tablice wierzchołków, kolorów itp. są przekazywane za
    // pomocą innych mechanizmów.
    m_MVPLocation = m_program.getUniformLocation("MVP");
    m_lightLocation = m_program.getUniformLocation("light");
    m_lightSizeLocation = m_program.getUniformLocation("lightSize");

    // Wczytujemy mapę z obrazka i generujemy z niej tablicę wierzcholków
    // którymi karmimy naszą kartę.
    m_map.load("../data/map.png");
	m_map.generate(10.0f, 10.0f);


	//object.loadObject();
	
}

void Game::cleanup() {
    //Czyszczenie...
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
        // Wyliczamy sobie naszą deltę, która będzie miała wpływ na wszelkie
        // występujące w grze animacje.
        delta = static_cast<float>(SDL_GetPerformanceCounter() - time) /
                                   SDL_GetPerformanceFrequency();
        time = SDL_GetPerformanceCounter();
        
        // Obsluga zdarzeń jest banalna. Wszystkie typy zdarzeń można znaleźć
        // w wiki SDL2.
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) 
                m_exit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                m_exit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_KP_ENTER)
                SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
        }

        // To czy klawisz jest przetrzymywany nie możemy uzyskać za pomocą
        // zdarzeń. Trzeba pobrać aktualny stan klawiatury.
        const Uint8 *keymap = SDL_GetKeyboardState(NULL);
        if (keymap[SDL_SCANCODE_RIGHT]) pos.x += delta;
        if (keymap[SDL_SCANCODE_LEFT]) pos.x -= delta;
        if (keymap[SDL_SCANCODE_UP]) pos.y += delta;
        if (keymap[SDL_SCANCODE_DOWN]) pos.y -= delta;

        // Pobieramy wymiary okna tak aby dopasować ratio w rzucie 
        // ortogonalnym. W ten sposób będziemy pewni, że obraz nie będzie w 
        // żaden sposób rozciągnięty.
        int w,h;
        SDL_GetWindowSize(m_window, &w, &h);
        float ratio = static_cast<float>(w)/h;
        float height = 1.0f;
        float width = height * ratio;

        // Za pomocą macierzy dokonujemy podstawowych przekształceń.
        // Najpierw ustalamy rzutowanie ortogonalne, następnie ustawiamy
        // kamerę.
        mat4 projection = ortho(-1 * width, width, -1 * height, height); 
        mat4 view = glm::lookAt(vec3(pos, 1.0f), vec3(pos, 0.0f), vec3(0,1,0));
        mat4 MVP = projection * view;

        // Przekazujemy macierz przekształcenia, pozycję światła i jego rozmiar
        // do shaderów.
        glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(MVP));
        glUniform2f(m_lightLocation, pos.x, pos.y);
        glUniform1f(m_lightSizeLocation, delta * 50.0f*2);

        // Czyścimy ekran na czarno
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rysujemy mapę. Magic happens here.
		m_map.draw();
		//object.draw();


        // Podmieniamy bufory wyświetlane przez menadżera okien.
        SDL_GL_SwapWindow(m_window);
    }
}
