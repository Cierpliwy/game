#include "FragmentShader.h"

void FragmentShader::create()
{
    m_id = glCreateShader(GL_FRAGMENT_SHADER);
}
