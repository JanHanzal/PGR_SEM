#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#include "pgr.h"
#include "Shader.h"



extern GLuint sun_texture;
extern GLuint earth_texture;
extern GLuint saturn_texture;
extern GLuint gold_texture;

extern Material default_planet_material;
extern Material gold;
extern Material pewter;
extern Material brass;


void initMaterials();
void createTextures();


#endif 
