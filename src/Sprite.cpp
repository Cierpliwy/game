#include "Sprite.h"

Sprite::Sprite()
{
    vao = 0;
    vbo = 0;
}

Sprite::~Sprite()
{
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
}

void Sprite::load(const char *filePath)
{
    texture.load(filePath);
}

void Sprite::generate(Rect<glm::vec3> rect)
{
    // Y or V is flipped by loading from SDL
    vertices[0].position = rect.a;
    vertices[0].uv = glm::vec2(0.0f,1.0f);
    vertices[1].position = rect.b;
    vertices[1].uv = glm::vec2(1.0f,1.0f);
    vertices[2].position = rect.c;
    vertices[2].uv = glm::vec2(1.0f,0.0f);
    vertices[3].position = rect.c;
    vertices[3].uv = glm::vec2(1.0f,0.0f);
    vertices[4].position = rect.d;
    vertices[4].uv = glm::vec2(0.0f,0.0f);
    vertices[5].position = rect.a;
    vertices[5].uv = glm::vec2(0.0f,1.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(ObjectVertex),
                 &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(ObjectVertex::POSITION, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(ObjectVertex), 
                          (void*)offsetof(ObjectVertex, position));
    glEnableVertexAttribArray(ObjectVertex::POSITION);

    glVertexAttribPointer(ObjectVertex::UV, 2, GL_FLOAT, GL_FALSE, 
                          sizeof(ObjectVertex), 
                          (void*)offsetof(ObjectVertex, uv));
    glEnableVertexAttribArray(ObjectVertex::UV);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw(GLuint texLocation)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id());
    glUniform1i(texLocation, 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
