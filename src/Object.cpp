#include "Object.h"


Object::Object(const char* mesh_path){
	strcpy (this->mesh_path,mesh_path);
}

bool Object::loadObject(){

	if(!ObjectLoader::loadSimpleObject(mesh_path, vertices, uvs, normals)){
		return false;
	}
	
    // Przekazujemy dane i sugerujemy by by³y one trzymane w pamiêci karty
    // graficznej poprzez GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
		         &vertices[0], GL_STATIC_DRAW);

	return true;
}

void Object::draw(){
	// Ta funkcja dotyczy obiektu VAO. Aktywujemy atrybut nr 0.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);
    // Mówimy ¿e powy¿ej u¿yty bufor bêdzie dostêpny poprzez atrybut nr 0.
    // Dane sk³adaj¹ siê z ci¹gu 3 liczb zmiennoprzecinkowych, które stanowi¹
    // punkt.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Z otrzymanych punktów chcemy rysowaæ trójk¹ty wykorzystuj¹c wszystkie
    // dane...
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    // Grzecznie zamykamy atrybut nr 0.
	glDisableVertexAttribArray(0);

}

Object::~Object(void)
{
}
