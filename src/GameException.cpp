#include "GameException.h"
#include "SDL.h"
#include "SDL_image.h"
#include "GL/glew.h"
using namespace std;

GameException::GameException(ModuleType type, const char *msg, int value)
{
    m_msg.clear();
    m_msg += "ERROR(";
    switch(type) {
        case SDL:
            m_msg += "SDL): ";
            m_msg += SDL_GetError();
            break;
        case SDL_IMAGE:
            m_msg += "SDL_IMAGE): ";
            m_msg += IMG_GetError();
            break;
        case GLEW:
            m_msg += "GLEW): ";
            m_msg += reinterpret_cast<const char*>(glewGetErrorString(value));
            break;
        case GL:
            m_msg += "GL): ";
            m_msg += reinterpret_cast<const char*>(gluErrorString(value));
            break;
        case INTERNAL:
            m_msg += "INTERNAL): ";
            break;
    }
    
    if (msg) {
        m_msg += " [";
        m_msg += msg;
        m_msg += "]";
    }

    m_msgPtr = m_msg.c_str();
}
