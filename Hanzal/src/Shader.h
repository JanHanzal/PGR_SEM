#include "pgr.h"

#ifndef __SHADER_H
#define __SHADER_H

#define VERT_SHADER "Shaders/defaultShader.vert"
#define FRAG_SHADER "Shaders/defaultShader.frag"

struct DirectionalLight
{
	glm::vec3	  position;
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
};

struct SpotLight
{
	glm::vec3	  position;
	glm::vec3	  direction;

	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;

	float         cosCutOff;
	float		  exponent;
};

struct PointLight
{
	glm::vec3	  position;
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;

	float		  radius;
	bool		  quadratic; //Linear if false
};

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
	
	//Light
	

} SCommonShaderProgram;

void setTransformUniforms(SCommonShaderProgram& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

void setMaterialUniforms(SCommonShaderProgram& shaderProgram, const Material material);

void initializeShaderPrograms(SCommonShaderProgram& shaderProgram);

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram);

#endif
