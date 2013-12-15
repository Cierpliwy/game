#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H
#include "Texture.h"

class Rendertarget
{
public:
    Rendertarget();
    ~Rendertarget();

    void create(unsigned int width, unsigned int height);

private:
    Texture texture;

};

#endif //RENDER_TARGET_H
