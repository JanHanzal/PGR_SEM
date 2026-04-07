#include "Shader.h"


#define VERT_SHADER "Shaders/defaultShader.vert"
#define FRAG_SHADER "Shaders/defaultShader.frag"


void setTransformUniforms(SCommonShaderProgram& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

	const glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

	glUniformMatrix4fv(shaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// just take 3x3 rotation part of the modelMatrix
	// we presume the last row contains 0,0,0,1
	const glm::mat4 modelRotationMatrix = glm::mat4(
		modelMatrix[0],
		modelMatrix[1],
		modelMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	const glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

	//or an alternative single-line method: 
	//glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat4(glm::mat3(modelRotationMatrix))));

	glUniformMatrix4fv(shaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void setMaterialUniforms(SCommonShaderProgram& shaderProgram, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture)
{
	glUniform3fv(shaderProgram.diffuseLocation, 1, glm::value_ptr(diffuse));
	glUniform3fv(shaderProgram.ambientLocation, 1, glm::value_ptr(ambient));
	glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(specular));
	glUniform1f(shaderProgram.shininessLocation, shininess);

	if (texture != 0) {
		glUniform1i(shaderProgram.useTextureLocation, 1);
		glUniform1i(shaderProgram.texSamplerLocation, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else {
		glUniform1i(shaderProgram.useTextureLocation, 0);
	}
}

void initializeShaderPrograms(SCommonShaderProgram& shaderProgram) 
{

	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, VERT_SHADER));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, FRAG_SHADER));

	shaderProgram.program = pgr::createProgram(shaderList);

	shaderProgram.posLocation = glGetAttribLocation(shaderProgram.program, "position");

	shaderProgram.PVMmatrixLocation = glGetUniformLocation(shaderProgram.program, "PVMmatrix");
	shaderProgram.VmatrixLocation = glGetUniformLocation(shaderProgram.program, "Vmatrix");
	shaderProgram.MmatrixLocation = glGetUniformLocation(shaderProgram.program, "Mmatrix");

	shaderProgram.normalLocation = glGetAttribLocation(shaderProgram.program, "normal");
	shaderProgram.texCoordLocation = glGetAttribLocation(shaderProgram.program, "texCoord");
	shaderProgram.normalMatrixLocation = glGetUniformLocation(shaderProgram.program, "normalMatrix");

	shaderProgram.ambientLocation = glGetUniformLocation(shaderProgram.program, "material.ambient");
	shaderProgram.diffuseLocation = glGetUniformLocation(shaderProgram.program, "material.diffuse");
	shaderProgram.specularLocation = glGetUniformLocation(shaderProgram.program, "material.specular");
	shaderProgram.shininessLocation = glGetUniformLocation(shaderProgram.program, "material.shininess");
	// texture
	shaderProgram.texSamplerLocation = glGetUniformLocation(shaderProgram.program, "texSampler");
	shaderProgram.useTextureLocation = glGetUniformLocation(shaderProgram.program, "material.useTexture");
}

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram)
{
	pgr::deleteProgramAndShaders(shaderProgram.program);
}