#ifndef __SHADER_H
#define __SHADER_H

#include "pgr.h"
#include "ShadersConfiguration.h"

extern float elapsedTime;


//============================================
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
//============================================

struct Material
{
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	GLuint        texture;
};

typedef struct _commonShaderProgram {
	GLuint program;          
	
	GLint posLocation;       
	GLint colorLocation;     
	GLint normalLocation;    
	GLint texCoordLocation;  
	
	GLint PVMmatrixLocation;  
	GLint VmatrixLocation;    
	GLint MmatrixLocation;    
	GLint normalMatrixLocation;

	// material 
	GLint diffuseLocation;    
	GLint ambientLocation;    
	GLint specularLocation;   
	GLint shininessLocation;  
	
	// texture
	GLint useTextureLocation;
	GLint texSamplerLocation;
	

	GLint activeLightningLocation;
	//Light - Directional, Spot, Point
	GLint dl_positionLocation;
	GLint dl_ambientLocation;
	GLint dl_diffuseLocation; 
	GLint dl_specularLocation;

	GLint sl_positionLocation;
	GLint sl_directionLocation;
	GLint sl_CutOffLocation;
	GLint sl_exponentLocation;
	GLint sl_diffuseLocation;    
	GLint sl_ambientLocation;    
	GLint sl_specularLocation;   

	GLint pl_positionLocation;
	GLint pl_diffuseLocation;    
	GLint pl_ambientLocation;    
	GLint pl_specularLocation;   
	GLint pl_radiusLocation;
	GLint pl_quadraticLocation;

} ObjectShader;

typedef struct SkyboxShader
{
	GLuint program;

	GLuint screenCoordsLocation;

	GLuint inversePVMatrixLocation;
	GLuint skyboxSamplerLocation;;
};

void setTransformUniforms(ObjectShader& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

void setMaterialUniforms(ObjectShader& shaderProgram, const Material material);

void setDirectionalLightUniforms(ObjectShader& shaderProgram, const DirectionalLight light);

void setSpotLightUniforms(ObjectShader& shaderProgram, const SpotLight light);

void setPointLightUniforms(ObjectShader& shaderProgram, const PointLight light);


void initializeObjectShaderProgram(ObjectShader& shaderProgram);

void initializeSkyboxShaderProgram(SkyboxShader& skyboxShader);


void cleanupShaderPrograms(ObjectShader& shaderProgram);

#endif
