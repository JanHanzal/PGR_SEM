#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#include "pgr.h"
#include "Shader.h"


extern GLuint default_planet_texture;
extern GLuint sun_texture;
extern GLuint earth_texture;
extern GLuint saturn_texture;
extern GLuint moon_texture;
extern GLuint neptune_texture;

extern GLuint gold_texture;
extern GLuint silver_texture;


extern Material default_planet_material;
extern Material gold;
extern Material pewter;
extern Material brass;


void initMaterials();
void createTextures();


#endif 
