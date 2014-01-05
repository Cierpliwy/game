#include "MapLights.h"


MapLights::MapLights(void)
{
}

bool MapLights::load(string _patch){
    std::ifstream t(_patch);
    if(!t.good()){
        return false;
    }
    string input_from_file((istreambuf_iterator<char>(t)),istreambuf_iterator<char>());
    string tmp;
    vector< string > split_vec;
	
    while(getline(t,tmp)){
        split_vec.push_back(tmp);
        tmp.clear();
    }

    for(string &line : split_vec){
        vector< string > values;
	    for(unsigned int i =0, j =0; i < line.size(); ++i){
            if((line.size() > i ) || (line[i]==';')){
                values.push_back(line.substr(j,i-j));
                j = i+1;
            }   
            i++;
        }
        if(values.size() != NUMBER_OF_VARIABLES_IN_FILE_LINE){
            return false;
        }
        lights.push_back(Light(static_cast<float>(atof(values[0].c_str())),
                               static_cast<float>(atof(values[1].c_str())),
                               static_cast<float>(atof(values[2].c_str()))));
    }

    return true;
}

void MapLights::draw(){

    for(Light &light : lights){
    
    }

}
MapLights::~MapLights(void)
{
}
