#include "Shader.h"


void setTransformUniforms(ObjectShader& shaderProgram, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, bool movingTexture)
{

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



	
	glm::mat4 textureMatrix = glm::mat4(1.0f);
	// Change texture transformation here ========================================================
	if (movingTexture)
	{
		textureMatrix = glm::translate(textureMatrix, glm::vec3(0.05f * elapsedTime, 0.05f * elapsedTime, 0.0f));
		textureMatrix = glm::scale(textureMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	//============================================================================================
	glUniformMatrix4fv(shaderProgram.textureMatrixLocation, 1, GL_FALSE, glm::value_ptr(textureMatrix));
}

void setMaterialUniforms(ObjectShader& shaderProgram, const Material material)
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

void setDirectionalLightUniforms(ObjectShader& shaderProgram, const DirectionalLight light)
{
	glUniform3fv(shaderProgram.dl_positionLocation, 
		1, 
		glm::value_ptr(glm::vec3( cos(elapsedTime), -sin(elapsedTime), cos(elapsedTime) * (sin(elapsedTime) - 1)))
	);
	glUniform3fv(shaderProgram.dl_ambientLocation, 1, glm::value_ptr(light.ambient));
	glUniform3fv(shaderProgram.dl_diffuseLocation, 1, glm::value_ptr(light.diffuse));
	glUniform3fv(shaderProgram.dl_specularLocation, 1, glm::value_ptr(light.specular));
	
}

void setSpotLightUniforms(ObjectShader& shaderProgram, const SpotLight light)
{
	float cutoff = std::min(60.0f, light.CutOff);
	cutoff = std::max(0.0f, cutoff);

	glUniform3fv(shaderProgram.sl_positionLocation,1,glm::value_ptr(light.position));
	glUniform3fv(shaderProgram.sl_directionLocation, 1, glm::value_ptr(light.diretion));
	glUniform3fv(shaderProgram.sl_ambientLocation, 1, glm::value_ptr(light.ambient));
	glUniform3fv(shaderProgram.sl_diffuseLocation, 1, glm::value_ptr(light.diffuse));
	glUniform3fv(shaderProgram.sl_specularLocation, 1, glm::value_ptr(light.specular));
	
	glUniform1f(shaderProgram.sl_CutOffLocation, cutoff);
	glUniform1f(shaderProgram.sl_exponentLocation, light.exponent);
}

void setPointLightUniforms(ObjectShader& shaderProgram, const PointLight light)
{
	glUniform3fv(shaderProgram.pl_positionLocation, 1, glm::value_ptr(light.position));
	glUniform3fv(shaderProgram.pl_ambientLocation, 1, glm::value_ptr(light.ambient));
	glUniform3fv(shaderProgram.pl_diffuseLocation, 1, glm::value_ptr(light.diffuse));
	glUniform3fv(shaderProgram.pl_specularLocation, 1, glm::value_ptr(light.specular));

	glUniform1f(shaderProgram.pl_radiusLocation, light.radius);
	glUniform1i(shaderProgram.pl_quadraticLocation, light.quadratic);
}

void initializeObjectShaderProgram(ObjectShader& shaderProgram) 
{

	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, OBJECT_VERT_SHADER));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, OBJECT_FRAG_SHADER));


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

	// Texture
	shaderProgram.texSamplerLocation = glGetUniformLocation(shaderProgram.program, "texSampler");
	shaderProgram.useTextureLocation = glGetUniformLocation(shaderProgram.program, "material.useTexture");
	shaderProgram.textureMatrixLocation = glGetUniformLocation(shaderProgram.program, "textureMatrix");

	shaderProgram.fogEnabledLocation = glGetUniformLocation(shaderProgram.program, "FOG_ENABLED");

	shaderProgram.isObjectSunLocation = glGetUniformLocation(shaderProgram.program, "isObjectSun");
	shaderProgram.activeLightningLocation = glGetUniformLocation(shaderProgram.program, "activeLightning");
	//Light - Directional
	shaderProgram.dl_positionLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.position");
	shaderProgram.dl_ambientLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.ambient");
	shaderProgram.dl_diffuseLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.diffuse");
	shaderProgram.dl_specularLocation = glGetUniformLocation(shaderProgram.program, "directionalLight.specular");

	//Light - Spot
	shaderProgram.sl_positionLocation = glGetUniformLocation(shaderProgram.program, "spotLight.position");
	shaderProgram.sl_directionLocation = glGetUniformLocation(shaderProgram.program, "spotLight.direction");
	shaderProgram.sl_CutOffLocation = glGetUniformLocation(shaderProgram.program, "spotLight.cutOff");
	shaderProgram.sl_exponentLocation = glGetUniformLocation(shaderProgram.program, "spotLight.exponent");
	shaderProgram.sl_ambientLocation = glGetUniformLocation(shaderProgram.program, "spotLight.ambient");
	shaderProgram.sl_diffuseLocation = glGetUniformLocation(shaderProgram.program, "spotLight.diffuse");
	shaderProgram.sl_specularLocation = glGetUniformLocation(shaderProgram.program, "spotLight.specular");

	//Light - Point
	shaderProgram.pl_positionLocation = glGetUniformLocation(shaderProgram.program, "pointLight.position");
	shaderProgram.pl_ambientLocation = glGetUniformLocation(shaderProgram.program, "pointLight.ambient");
	shaderProgram.pl_diffuseLocation = glGetUniformLocation(shaderProgram.program, "pointLight.diffuse");
	shaderProgram.pl_specularLocation = glGetUniformLocation(shaderProgram.program, "pointLight.specular");
	shaderProgram.pl_radiusLocation = glGetUniformLocation(shaderProgram.program, "pointLight.radius");
	shaderProgram.pl_quadraticLocation = glGetUniformLocation(shaderProgram.program, "pointLight.quadratic");
}

void initializeSkyboxShaderProgram(SkyboxShader& skyboxShader)
{
	std::vector<GLuint> shaders;

	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, SKYBOX_VERT_SHADER));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, SKYBOX_FRAG_SHADER));
	

	skyboxShader.program = pgr::createProgram(shaders);
	

	skyboxShader.screenCoordsLocation = glGetAttribLocation(skyboxShader.program, "screenCoords");

	skyboxShader.inversePVMatrixLocation = glGetUniformLocation(skyboxShader.program, "inversePVMatrix");
	skyboxShader.skyboxSamplerLocation = glGetUniformLocation(skyboxShader.program, "skyboxSampler");
	skyboxShader.fogEnabledLocation = glGetUniformLocation(skyboxShader.program, "FOG_ENABLED");
}

void initializeAnimatedBillboardShaderProgram(AnimatedBillboardShaderProgram& animBillboardShader)
{
	std::vector<GLuint> shaders;

	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, FIRE_VERT_SHADER));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, FIRE_FRAG_SHADER));

	animBillboardShader.program = pgr::createProgram(shaders);

	animBillboardShader.posLocation = glGetAttribLocation(animBillboardShader.program, "position");
	animBillboardShader.texCoordLocation = glGetAttribLocation(animBillboardShader.program, "texCoord");

	animBillboardShader.PVMmatrixLocation = glGetUniformLocation(animBillboardShader.program, "PVMmatrix");
	animBillboardShader.timeLocation = glGetUniformLocation(animBillboardShader.program, "time");
	animBillboardShader.texSamplerLocation = glGetUniformLocation(animBillboardShader.program, "texSampler");
}

