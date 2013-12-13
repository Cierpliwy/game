#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <vector>
#include <string>

class Shader
{
public:
    Shader();
    virtual ~Shader();
    virtual void load(const char *filePath);
    virtual void compile();
    GLenum getID() const { return m_id; }

protected:
    virtual void create() = 0;
    virtual void setSource();
    virtual void free();
    virtual void checkErrors();

    GLenum m_id;
    std::vector<char> m_code;
    std::string m_name;
};

#endif //SHADER_H
