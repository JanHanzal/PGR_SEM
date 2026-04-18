#include "Materials.h"


#define SUN_TEXTURE_PATH "Data/Sun2.jpg"
#define EARTH_TEXTURE_PATH "Data/Earth.jpg"
#define SATURN_TEXTURE_PATH "Data/Saturn.jpg"

//Textures
GLuint sun_texture;
GLuint earth_texture;
GLuint saturn_texture;


//Materials
Material default_planet_material;

void createTextures()
{
	sun_texture = pgr::createTexture(SUN_TEXTURE_PATH);
	earth_texture = pgr::createTexture(EARTH_TEXTURE_PATH);
	saturn_texture = pgr::createTexture(SATURN_TEXTURE_PATH);
}

void initMaterials()
{
	createTextures();


	default_planet_material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	default_planet_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	default_planet_material.specular = glm::vec3(1.0f);
	default_planet_material.shininess = 0.0f;
	default_planet_material.texture = sun_texture;

}