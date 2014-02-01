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
    void setScreenSize(unsigned screenWidth, unsigned screenHeight) {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;
    }

    void setTime(float time) {
        m_time = time;
    }

private:
    Sprite m_sprite;
    Program m_program;
    VertexShader m_vertex;
    FragmentShader m_fragment;
    GLuint m_texLocation;
    GLuint m_charLocation;
    GLuint m_pvmLocation;
    GLuint m_timeLocation;
    GLuint m_blackLocation;
    float m_time;
    unsigned m_screenWidth;
    unsigned m_screenHeight;
};

#endif //FONT_H
