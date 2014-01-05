#ifndef DEBUG_H
#define DEBUG_H

#ifdef _WIN32
#include "Windows.h"
#include "WinBase.h"
#endif

#include <iostream>
#include <GL/glew.h>

void gameDebugCallbackARB(GLenum source, GLenum type, GLuint id, 
                          GLenum severity, GLsizei length, 
                          const GLchar *message, GLvoid *userParam);

void gameDebugCallbackAMD(GLuint id, GLenum category, GLenum severity,
                          GLsizei length, const GLchar *message, 
                          GLvoid* userParam);

#endif //DEBUG_H