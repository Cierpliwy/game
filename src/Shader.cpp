#include "Shader.h"
#include "GameException.h"
#include <fstream>
#include <sstream>
using namespace std;

Shader::Shader() {
    m_id = 0;
}

Shader::~Shader()
{
    free();
}

void Shader::free()
{
    m_code.clear();
    glDeleteShader(m_id);
    m_id = 0;
}

void Shader::load(const char *filePath)
{
    free();
    m_name.clear();
    m_name += filePath;

    ifstream file(filePath, ios::binary | ios::in);
    if (file) {
        file.seekg(0, ios::end);
        unsigned int s = static_cast<unsigned int>(file.tellg());
        file.seekg(0, ios::beg);
        m_code.resize(s+1);
        file.read(&m_code[0], s);
        file.close();
        m_code[s] = '\0';
    } else throw GameException(GameException::INTERNAL, filePath);
}

void Shader::compile() {
    if (m_id) 
        throw GameException(GameException::INTERNAL, 
                            "Cannot compile shader twice in a row");
    create();
    setSource();
    glCompileShader(m_id);

    checkErrors();
}

void Shader::setSource() {
    const char *source = &m_code[0];
    glShaderSource(m_id, 1, &source, NULL);
}

void Shader::checkErrors()
{
    stringstream ss;
    ss << m_name << ": ";
    GLint result;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &result);
        vector<char> log(result);
        glGetShaderInfoLog(m_id, result, NULL, &log[0]);
        ss << &log[0];
        throw GameException(GameException::INTERNAL, ss.str().c_str()); 
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) 
        throw GameException(GameException::GL, "Shader", err);
}
