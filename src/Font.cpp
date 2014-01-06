#include "Font.h"
using namespace glm;

void Font::load(const std::string &fontFile)
{
    m_sprite.load(fontFile.c_str()); 
    m_sprite.generate(Rect<vec3>(
        vec3(-1.0f, -1.0f, 0.0f),
        vec3(1.0f, -1.0f, 0.0f),
        vec3(1.0f, 1.0f, 0.0f),
        vec3(-1.0f, 1.0f, 0.0f)));

    glBindTexture(GL_TEXTURE_2D, m_sprite.getTexture().id());
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_vertex.load("../data/textVertex.glsl");
    m_vertex.compile();
    m_fragment.load("../data/textFragment.glsl");
    m_fragment.compile();
    m_program.create();
    m_program.setVertexShader(m_vertex);
    m_program.setFragmentShader(m_fragment);
    m_program.link();
    m_texLocation = m_program.getUniformLocation("tex");
    m_charLocation = m_program.getUniformLocation("char");
    m_pvmLocation = m_program.getUniformLocation("PVM");
}

void Font::print(const std::string &text, float size, float x, float y)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    m_program.use();
    glUniform1i(m_texLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sprite.getTexture().id());
    float x2 = x;
    for(unsigned int pos = 0; pos < text.size(); ++pos) {
        if (text[pos] == '\n') {
            x2 = x;
            y -= size*1.5;
            continue;
        }
        mat4 a = ortho(-1.0f,1.0f,-1.0f,1.0f,-100.0f,100.0f);
        mat4 b = translate(a, vec3(x2,y,0));
        mat4 c = scale(b, vec3(size,size,0.0f));
        glUniform1f(m_charLocation, static_cast<unsigned int>(text.at(pos)));
        glUniformMatrix4fv(m_pvmLocation, 1, GL_FALSE, value_ptr(c));
        m_sprite.draw();
        x2 += size;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
