#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#include "pgr.h"
#include "Shader.h"



extern GLuint sun_texture;
extern GLuint earth_texture;
extern GLuint saturn_texture;

extern Material mat;

void initMaterials();
void createTextures();


#endif 
