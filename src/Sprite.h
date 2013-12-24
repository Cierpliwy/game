#ifndef SPRITE_H
#define SPRITE_H
#include "Utils.h"
#include "Object.h"
#include "Texture.h"

class Sprite
{
public:
    Sprite();
    ~Sprite();

    void load(const char *texFilePath);
    void generate(Rect<glm::vec3> rect);
    GLuint getTextureID() {return texture.id();}
    void draw();

private:

    ObjectVertex vertices[6];
    Texture texture;
    GLuint vbo, vao;
};

#endif //SPRITE_H
