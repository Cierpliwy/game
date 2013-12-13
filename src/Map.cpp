#include "Map.h"
#include "GameException.h"
#include "SDL_image.h"
using namespace std;

Map::Map() : m_width(0), m_height(0), m_surface(NULL), m_vao(0), m_vbo(0){
}

Map::~Map(){
    free();
}

void Map::free(){
    if (m_surface) SDL_FreeSurface(m_surface);
}

void Map::load(const char *path){
    m_path = path;
    free();

    m_surface = IMG_Load(path);
    if (!m_surface) 
        throw GameException(GameException::SDL_IMAGE, path);
}

// Kod pobierający wartość z piksela o danych koordynatach. Jeżeli
// dojdą kanały RGB etc. Trzeba będzie to lekko zmodyfikować.
unsigned char Map::getPixel(unsigned int x, unsigned int y){
	unsigned char bpp = m_surface->format->BytesPerPixel;
	return static_cast<unsigned char*>(m_surface->pixels)
		                              [y * m_surface->pitch + x * bpp];
}

// Funkcja dodaje jeden wierzchołek do listy zmiennych float.
void Map::addPoint(unsigned int x, unsigned int y){
	float pw = m_width / m_surface->w;
	float ph = m_height / m_surface->h;
	m_vertices.push_back(x*pw);
	m_vertices.push_back(-1*(y*ph));
	m_vertices.push_back(0.0f);
}

// Generujemy mapę, przekształcając ją w listę wierzchołków z postaci bitmapy.
void Map::generate(float width, float height) {
	m_width = width;
    m_height = height;
	m_vertices.clear();

	for(int i = 0; i < m_surface->w; ++i) {
		for(int j = 0; j < m_surface->h; ++j) {
			if (getPixel(i,j)) {
                // Przypadek prosty. Jeżeli piksel jest wypełniony to rysujemy
                // kwardrat poprzez umieszczenie 2 trójkątów. Kolejność punktów
                // ma znaczenie. Trójkąt będzie widoczny gdy kolejność jego
                // punktów będzie zgodna z ruchem wskazówek zegara.
				addPoint(i,j);
				addPoint(i+1,j);
				addPoint(i+1,j+1);
				addPoint(i+1,j+1);
				addPoint(i,j+1);
				addPoint(i,j);
			} else {
                // Przypadek gdy piksel jest pusty. Należy sprawdzić jego
                // otoczenie i w zależności od niego zdecydować, czy trójkąt
                // wygładzający powinien się pojawić.
				unsigned char mask = 0;
				if (j-1 >= 0 && getPixel(i, j-1)) mask |= 12;
				if (j+1 < m_surface->h && getPixel(i, j+1)) mask |= 3;
				if (i-1 >= 0 && getPixel(i-1, j)) mask |= 9;
				if (i+1 < m_surface->w && getPixel(i+1, j)) mask |= 6;

				if (mask == 14 || mask == 13 || mask == 11 || mask == 7) {
					if (mask & 8) addPoint(i,j);
					if (mask & 4) addPoint(i+1,j);
					if (mask & 2) addPoint(i+1,j+1);
					if (mask & 1) addPoint(i,j+1);
				}
			}
		}
	}

    // Tworzymy tzw. Vertex Array Object. Zawiera on informacje w jaki sposób
    // są przesyłane strumienie danych do karty graficznej. Ustawione później
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

    // Generujemy bufor dla wierzchołków. Tzw. Vertex Buffer Object. Będzie
    // on przechowywał dane przekazanych wierzchołków oraz informacje w jaki
    // sposób mają one być przechowywane.
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Przekazujemy dane i sugerujemy by były one trzymane w pamięci karty
    // graficznej poprzez GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat),
		         &m_vertices[0], GL_STATIC_DRAW);
}

void Map::draw()
{
    // Ta funkcja dotyczy obiektu VAO. Aktywujemy atrybut nr 0.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, m_vbo);
    // Mówimy że powyżej użyty bufor będzie dostępny poprzez atrybut nr 0.
    // Dane składają się z ciągu 3 liczb zmiennoprzecinkowych, które stanowią
    // punkt.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Z otrzymanych punktów chcemy rysować trójkąty wykorzystując wszystkie
    // dane...
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    // Grzecznie zamykamy atrybut nr 0.
	glDisableVertexAttribArray(0);
}
