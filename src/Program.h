#ifndef PROGRAM_H
#define PROGRAM_H
#include "FragmentShader.h"
#include "VertexShader.h"

class Program
{
public:
    Program();
    ~Program();

    void create();
    void setVertexShader(const VertexShader& vertex);
    void setFragmentShader(const FragmentShader& vertex);
    GLuint getUniformLocation(const char *name) const;
    void link();
    void use() const;

private:
    void checkErrors();

    const VertexShader* m_vertex;
    const FragmentShader* m_fragment;
    GLenum m_id;
};

#endif //PROGRAM_H
