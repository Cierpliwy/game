#ifndef FONT_H
#define FONT_H
#include "Sprite.h"
#include "Program.h"
#include "VertexShader.h"
#include "FragmentShader.h"

class Font
{
public:
    void load(const std::string &fontFile);
    void print(const std::string &text, float size, float x, float y);

private:
    Sprite m_sprite;
    Program m_program;
    VertexShader m_vertex;
    FragmentShader m_fragment;
    GLuint m_texLocation;
    GLuint m_charLocation;
    GLuint m_pvmLocation;
};

#endif //FONT_H
