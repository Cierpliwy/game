#pragma once

#include "GL/glew.h"
#include "ObjectLoader.h"
#include <stdio.h>
#include <string.h>

class Object
{
	char mesh_path[200];
	GLuint vertexbuffer;
	GLuint uvbuffer;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	glm::vec2 position;

public:
	Object(const char* mesh_path);
	/// £aduje Obiekt z pamiêci oraz zwraca czy siê uda³o
	bool loadObject();
	void draw();
	/// ustawia pozycje obiektu
	void setPosition(glm::vec2 &position){this->position = position;}
	glm::vec2 getPosition(){return position;}
	~Object(void);
};

