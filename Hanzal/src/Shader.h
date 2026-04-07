#include "pgr.h"

#ifndef __SHADER_H
#define __SHADER_H

typedef struct _commonShaderProgram {
	// identifier for the shader program
	GLuint program;          // = 0;
	// vertex attributes locations
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordLocation;  // = -1;
	// uniforms locations
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
	// spotlight related uniforms
} SCommonShaderProgram;


void setTransformUniforms(SCommonShaderProgram& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

void setMaterialUniforms(SCommonShaderProgram& shaderProgram, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture);

void initializeShaderPrograms(SCommonShaderProgram& shaderProgram);

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram);

#endif
