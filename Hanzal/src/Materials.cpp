#include "Materials.h"


#define SUN_TEXTURE_PATH "Data/Sun2.jpg"
#define EARTH_TEXTURE_PATH "Data/Earth.jpg"
#define SATURN_TEXTURE_PATH "Data/Saturn.jpg"
#define GOLD_TEXTURE_PATH "Data/Gold.jpg"
#define SILVER_TEXTURE_PATH "Data/Silver.jpg"

//Textures
GLuint sun_texture;
GLuint earth_texture;
GLuint saturn_texture;
GLuint gold_texture;
GLuint silver_texture;


//Materials
Material default_planet_material;
Material gold;
Material pewter;
Material brass;

void createTextures()
{
	sun_texture = pgr::createTexture(SUN_TEXTURE_PATH);
	earth_texture = pgr::createTexture(EARTH_TEXTURE_PATH);
	saturn_texture = pgr::createTexture(SATURN_TEXTURE_PATH);
	gold_texture = pgr::createTexture(GOLD_TEXTURE_PATH);
	silver_texture = pgr::createTexture(SILVER_TEXTURE_PATH);
}

void initMaterials()
{
	createTextures();


	default_planet_material.ambient = glm::vec3(1.0f);
	default_planet_material.diffuse = glm::vec3(1.0f);
	default_planet_material.specular = glm::vec3(1.0f);
	default_planet_material.shininess = 0.0f;
	default_planet_material.texture = sun_texture;

	gold.ambient = glm::vec3(0.24725, 0.1995, 0.0745);
	gold.diffuse = glm::vec3(0.75164, 0.60648, 0.22648);
	gold.specular = glm::vec3(0.628281, 0.555802, 0.366065);
	gold.shininess = 52.2;
	gold.texture = gold_texture;

	pewter.ambient = glm::vec3(0.3333, 0.3333, 0.5216);
	pewter.diffuse = glm::vec3(0.4275, 0.4706, 0.5412);
	pewter.specular = glm::vec3(0.1059, 0.0588, 0.1137);
	pewter.shininess = 9.85;
	pewter.texture = gold_texture;

	brass.ambient = glm::vec3(0.9922, 0.9412, 0.8079);
	brass.diffuse = glm::vec3(0.7804, 0.5686, 0.1137);
	brass.specular = glm::vec3(0.3294, 0.2235, 0.0275);
	brass.shininess = 27.89;
	brass.texture = silver_texture;

}