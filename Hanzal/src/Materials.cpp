#include "Materials.h"


#define SUN_TEXTURE_PATH "Data/Sun.jpg"
#define EARTH_TEXTURE_PATH "Data/Earth.jpg"
#define SATURN_TEXTURE_PATH "Data/Saturn.jpg"

//Textures
GLuint sun_texture;
GLuint earth_texture;
GLuint saturn_texture;


//Materials
Material mat;

void createTextures()
{
	sun_texture = pgr::createTexture(SUN_TEXTURE_PATH);
	earth_texture = pgr::createTexture(EARTH_TEXTURE_PATH);
	saturn_texture = pgr::createTexture(SATURN_TEXTURE_PATH);
}

void initMaterials()
{
	createTextures();


	mat.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	mat.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	mat.specular = glm::vec3(1.0f);
	mat.shininess = 0.0f;
	mat.texture = sun_texture;

}