#include "pgr.h"

#ifndef __SHADER_H
#define __SHADER_H

#define VERT_SHADER "Shaders/defaultShader.vert"
#define FRAG_SHADER "Shaders/defaultShader.frag"

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
	float CutOff;
	float exponent;

	//PointLight params
	float radius;
	bool quadratic;
};

struct PointLight {
	glm::vec3  position;
	glm::vec3 diretion;

	glm::vec3  ambient;
	glm::vec3  diffuse;
	glm::vec3  specular;

	//PointLight params
	float radius;
	bool quadratic;
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
	GLuint program;          // = 0;
	
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordLocation;  // = -1;
	
	GLint PVMmatrixLocation;    // = -1;
	GLint VmatrixLocation;      // = -1;  view/camera matrix
	GLint MmatrixLocation;      // = -1;  modeling matrix
	GLint normalMatrixLocation; // = -1;  inverse transposed Mmatrix

	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;
	
	// texture
	GLint useTextureLocation; // = -1; 
	GLint texSamplerLocation; // = -1;
	
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

} SCommonShaderProgram;

void setTransformUniforms(SCommonShaderProgram& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

void setMaterialUniforms(SCommonShaderProgram& shaderProgram, const Material material);

void setDirectionalLightUniforms(SCommonShaderProgram& shaderProgram, const DirectionalLight light);

void initializeShaderPrograms(SCommonShaderProgram& shaderProgram);

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram);

#endif
