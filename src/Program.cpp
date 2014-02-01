#include "Program.h"
#include "GameException.h"
using namespace std;

Program::Program() :
    m_vertex(NULL),
    m_fragment(NULL),
    m_id(0)
{
}

Program::~Program()
{
    if (m_vertex)
        glDetachShader(m_id, m_vertex->getID());
    if (m_fragment)
        glDetachShader(m_id, m_fragment->getID());

    glDeleteProgram(m_id);
}

void Program::create() {
    glDeleteProgram(m_id);
    m_id = glCreateProgram();
    if (!m_id) throw GameException(GameException::INTERNAL, 
                                   "Cannot create program");
}

void Program::link() {
    glLinkProgram(m_id);
    checkErrors();
}

void Program::checkErrors()
{
    GLint result;
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &result);
        vector<char> log(result);
        glGetProgramInfoLog(m_id, result, NULL, &log[0]);
        throw GameException(GameException::INTERNAL,
                            &log[0]); 
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) 
        throw GameException(GameException::GL, "Program", err); 
}

void Program::setVertexShader(const VertexShader &vertex) {
    if (m_vertex)
        glDetachShader(m_id, m_vertex->getID());
    m_vertex = &vertex;
    glAttachShader(m_id, m_vertex->getID());
}

void Program::setFragmentShader(const FragmentShader &fragment) {
    if (m_fragment)
        glDetachShader(m_id, m_fragment->getID());
    m_fragment = &fragment;
    glAttachShader(m_id, m_fragment->getID());
}

void Program::use() const {
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (static_cast<GLint>(m_id) != currentProgram)
        glUseProgram(m_id);
}

GLuint Program::getUniformLocation(const char *name) const
{
    return glGetUniformLocation(m_id, name);
}

void Program::validate() const
{
    GLint result;
    glValidateProgram(m_id);
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
    if (result != GL_TRUE) {
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &result);
        vector<char> log(result);
        glGetProgramInfoLog(m_id, result, NULL, &log[0]);
        throw GameException(GameException::INTERNAL,
                            &log[0]); 
    }
}
