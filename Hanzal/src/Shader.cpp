#include "Shader.h"


void setTransformUniforms(SCommonShaderProgram& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

	const glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

	glUniformMatrix4fv(shaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	const glm::mat4 modelRotationMatrix = glm::mat4(
		modelMatrix[0],
		modelMatrix[1],
		modelMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	const glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

	glUniformMatrix4fv(shaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void setMaterialUniforms(SCommonShaderProgram& shaderProgram, const Material material)
{
	glUniform3fv(shaderProgram.diffuseLocation, 1, glm::value_ptr(material.diffuse));
	glUniform3fv(shaderProgram.ambientLocation, 1, glm::value_ptr(material.ambient));
	glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(material.specular));
	glUniform1f(shaderProgram.shininessLocation, material.shininess);

	if (material.texture != 0) {
		glUniform1i(shaderProgram.useTextureLocation, true);
		glUniform1i(shaderProgram.texSamplerLocation, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, material.texture);
	}
	else {
		glUniform1i(shaderProgram.useTextureLocation, false);
	}
}

void setDirectionalLightUniforms(SCommonShaderProgram& shaderProgram, const DirectionalLight light)
{
	glUniform3fv(shaderProgram.dl_positionLocation, 1, glm::value_ptr(light.position));
	glUniform3fv(shaderProgram.dl_ambientLocation, 1, glm::value_ptr(light.ambient));
	glUniform3fv(shaderProgram.dl_diffuseLocation, 1, glm::value_ptr(light.diffuse));
	glUniform3fv(shaderProgram.dl_specularLocation, 1, glm::value_ptr(light.specular));
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

	//Light - Directional, Spot, Point
	shaderProgram.dl_positionLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.position");
	shaderProgram.dl_ambientLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.ambient");
	shaderProgram.dl_diffuseLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.diffuse");
	shaderProgram.dl_specularLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.specular");

	
	shaderProgram.sl_positionLocation = glGetUniformLocation(shaderProgram.program, "spotLight.position");
	shaderProgram.sl_directionLocation = glGetUniformLocation(shaderProgram.program, "spotLight.direction");
	shaderProgram.sl_CutOffLocation = glGetUniformLocation(shaderProgram.program, "spotLight.cutOff");
	shaderProgram.sl_exponentLocation = glGetUniformLocation(shaderProgram.program, "spotLight.exponent");
	shaderProgram.sl_ambientLocation = glGetUniformLocation(shaderProgram.program, "spotLight.ambient");
	shaderProgram.sl_diffuseLocation = glGetUniformLocation(shaderProgram.program, "spotLight.diffuse");
	shaderProgram.sl_specularLocation = glGetUniformLocation(shaderProgram.program, "spotLight.specular");

	shaderProgram.pl_positionLocation = glGetUniformLocation(shaderProgram.program, "pointLight.position");
	shaderProgram.pl_ambientLocation = glGetUniformLocation(shaderProgram.program, "pointLight.ambient");
	shaderProgram.pl_diffuseLocation = glGetUniformLocation(shaderProgram.program, "pointLight.diffuse");
	shaderProgram.pl_specularLocation = glGetUniformLocation(shaderProgram.program, "pointLight.specular");
	shaderProgram.pl_radiusLocation = glGetUniformLocation(shaderProgram.program, "pointLight.radius");
	shaderProgram.pl_quadraticLocation = glGetUniformLocation(shaderProgram.program, "pointLight.quadratic");
	
}

void cleanupShaderPrograms(SCommonShaderProgram& shaderProgram)
{
	pgr::deleteProgramAndShaders(shaderProgram.program);
}