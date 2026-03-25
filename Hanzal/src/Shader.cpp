#include "Shader.h"

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


void setTransformUniforms(SCommonShaderProgram* shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	const glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(shaderProgram->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
}


void initializeShaderPrograms(SCommonShaderProgram& shaderProgram) 
{

	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER, colorVertexShaderSrc));
	shaderList.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER, colorFragmentShaderSrc));

	shaderProgram.program = pgr::createProgram(shaderList);

	shaderProgram.posLocation = glGetAttribLocation(shaderProgram.program, "position");
	shaderProgram.colorLocation = glGetAttribLocation(shaderProgram.program, "color");
	shaderProgram.PVMmatrixLocation = glGetUniformLocation(shaderProgram.program, "PVMmatrix");
}

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram)
{
	pgr::deleteProgramAndShaders(shaderProgram.program);
}