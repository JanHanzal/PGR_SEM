#include "pgr.h"

#ifndef __SHADER_H
#define __SHADER_H

typedef struct _commonShaderProgram {
	// identifier for the shader program
	GLuint program;          // = 0;
	// vertex attributes locations
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint PVMmatrixLocation;    // = -1;
} SCommonShaderProgram;


const std::string colorVertexShaderSrc(
	"#version 140\n"
	"uniform mat4 PVMmatrix;\n"
	"in vec3 position;\n"
	"in vec3 color;\n"
	"smooth out vec4 theColor;\n"
	"void main() {\n"
	"  gl_Position = PVMmatrix * vec4(position, 1.0);\n"
	"  theColor = vec4(color, 1.0);\n"
	"}\n"
);

const std::string colorFragmentShaderSrc(
	"#version 140\n"
	"smooth in vec4 theColor;\n"
	"out vec4 outputColor;\n"
	"void main() {\n"
	"  outputColor = theColor;\n"
	"}\n"
);

void setTransformUniforms(SCommonShaderProgram* shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

void initializeShaderPrograms(SCommonShaderProgram& shaderProgram);

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram);

#endif
