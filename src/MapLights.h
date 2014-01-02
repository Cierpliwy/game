#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

#define NUMBER_OF_VARIABLES_IN_FILE_LINE 3

//class which is in charge of lights on map (maybe this will inherit from something else)
class MapLights
{
    struct Light{
        Light(float _x, float _y, float _radius): x(_x), y(_y), radius(_radius){
        }
        float x,y,radius;
    };
    vector<Light> lights;
public:
    MapLights(void);
    bool load(string _patch);
    void draw();
    ~MapLights(void);
};

