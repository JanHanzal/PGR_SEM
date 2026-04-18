#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "pgr.h"


struct DirectionalLight {
	glm::vec3  position;
	glm::vec3  ambient;
	glm::vec3  diffuse;
	glm::vec3  specular;
};

struct SpotLight {
	glm::vec3  position;
	glm::vec3 diretion;

	glm::vec3  ambient;
	glm::vec3  diffuse;
	glm::vec3  specular;

	//SpotLight
	float CutOff;	// In degrees
	float exponent;
};

struct PointLight {
	glm::vec3  position;

	glm::vec3  ambient;
	glm::vec3  diffuse;
	glm::vec3  specular;

	//PointLight params
	float radius;
	bool quadratic;		// Is fading quadratic?
};


#endif 
