#include "Renderer.h"

void Renderer::shadersInit()
{
	initializeObjectShaderProgram(objectShader);
	initializeSkyboxShaderProgram(skyboxShader);
}

void Renderer::loadMeshes()
{
	geometry.resize(OBJECT_TYPE_SIZE);

	const char* earth = "Data/Earth.obj";

	const char* sphere = "Data/Sphere.obj";
	loadSingleMesh(sphere, objectShader, &geometry[SPHERE]);
		
	const char* qhoul = "Data/ghoul.obj";
	loadSingleMesh(qhoul, objectShader, &geometry[GHOUL]);

	CHECK_GL_ERROR();
}

void Renderer::setUpLights()
{
	directionalLight = {
		glm::vec3(0.0f, 0.0f, 1.0f),// Position 
		glm::vec3(0.1f),			// Ambient
		glm::vec3(1.0f),			// Diffuse
		glm::vec3(0.5f)				// Specular
	};

	spotLight = {
		glm::vec3(1.0f),			// Position 
		glm::vec3(1.0f, 0.0f, 0.0f),// Direction

		glm::vec3(0.1f),			// Ambient
		glm::vec3(1.0f),			// Diffuse
		glm::vec3(0.5f),			// Specular

		45.0f,						// Cutoff - in degrees
		1.0f						// Exponent
	};


	pointLight = {
		glm::vec3(0.0f),			// Position
		
		glm::vec3(0.1f),			// Ambient
		glm::vec3(1.0f),			// Diffuse
		glm::vec3(0.5f),			// Specular

		1000.0f,					// Radius
		false						// Is fading quadratic?
	};
}

Renderer::~Renderer()
{
	for (MeshGeometry* mesh : geometry)
	{
		cleanupGeometry(mesh);
	}
	cleanupShaderPrograms(objectShader);
}

void Renderer::Init()
{
	shadersInit();
	setUpLights();
	loadMeshes();
	initSpaceMonkey();
	initSkybox();
}

void Renderer::initSpaceMonkey()
{
	glGenVertexArrays(1, &spaceMonkey.vao);
	glBindVertexArray(spaceMonkey.vao);

	glGenBuffers(1, &spaceMonkey.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, spaceMonkey.vbo);

	GLsizei stride = 8 * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, pgr::monkeyData.nVertices * stride, pgr::monkeyData.verticesInterleaved, GL_STATIC_DRAW);

	glGenBuffers(1, &spaceMonkey.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spaceMonkey.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pgr::monkeyData.nTriangles * 3 * sizeof(unsigned int), pgr::monkeyData.triangles, GL_STATIC_DRAW);
	

	glEnableVertexAttribArray(objectShader.posLocation);
	glVertexAttribPointer(objectShader.posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(objectShader.normalLocation);
	glVertexAttribPointer(objectShader.normalLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	

	glEnableVertexAttribArray(objectShader.texCoordLocation);
	glVertexAttribPointer(objectShader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	

	glBindVertexArray(0);

	spaceMonkey.monkey_texture = pgr::createTexture("Data/Sun.jpg");
}

void Renderer::initSkybox()
{
	skyBox = new MeshGeometry;

	static const float screenCoords[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,
	};

	glGenVertexArrays(1, &skyBox->vertexArrayObject);
	glBindVertexArray(skyBox->vertexArrayObject);

	glGenBuffers(1, &skyBox->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, skyBox->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(skyboxShader.screenCoordsLocation);
	glVertexAttribPointer(skyboxShader.screenCoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();

	skyBox->numTriangles = 2;

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &skyBox->material.texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->material.texture);

	const char* names[] = {"px", "nx", "py", "ny", "pz", "nz"};
	GLuint textures[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++)
	{
		std::string path = std::string("Data/") + names[i] + ".jpg";
		if (!pgr::loadTexImage2D(path, textures[i]))
			pgr::dieWithError("Loading skybox failed!");
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();
}

void Renderer::changeActiveLightning()//ActiveLightning _activeLightning)
{
	activeLightning = static_cast<ActiveLightning>((activeLightning + 1) % 3);
	//activeLightning = _activeLightning;
}

void Renderer::changeFogEnablement()
{
	FOG_ENABLED = !FOG_ENABLED;
}

void Renderer::changeDirectionalLightParams(DirectionalLight newDirLight)
{
	directionalLight = newDirLight;
}

void Renderer::changeSpotLightParams(SpotLight _spotLight)
{
	spotLight = _spotLight;
}

void Renderer::changePointLightParams(PointLight _pointLight)
{
	pointLight = _pointLight;
}


void Renderer::renderSpaceMonkey(Material material, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glUseProgram(objectShader.program);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	setTransformUniforms(
		objectShader,
		modelMatrix,
		viewMatrix,
		projectionMatrix,
		false
	);
	

	setMaterialUniforms(objectShader, material);
	setDirectionalLightUniforms(objectShader, directionalLight);
	setSpotLightUniforms(objectShader, spotLight);
	setPointLightUniforms(objectShader, pointLight);

	glBindVertexArray(spaceMonkey.vao);
	glDrawElements(GL_TRIANGLES, pgr::monkeyData.nTriangles * 3, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void Renderer::renderObject(ObjectType objectType,
	const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix,
	bool wireframed, bool movingTexture, bool isObjectSun)
{
	if (objectType == OBJECT_TYPE_SIZE)
		return;
	
	if (objectType == MONKEY)
	{
		Material mat = {
			glm::vec3(1.0f, 1.0f, 1.0f),  // Ambient
			glm::vec3(1.0f, 1.0f, 1.0f),  // Diffuse
		    glm::vec3(1.0f),              // Specular
			0.0f,                         // Shininess
			NULL
		};

		renderSpaceMonkey(mat, modelMatrix, viewMatrix, projectionMatrix);
		return;
	}

	glUseProgram(objectShader.program);

 	const MeshGeometry* mesh = geometry[objectType];

	setTransformUniforms(
		objectShader,
		modelMatrix,
		viewMatrix,
		projectionMatrix,
		movingTexture
	);

	setMaterialUniforms(objectShader, mesh->material);
	setDirectionalLightUniforms(objectShader, directionalLight);
	setSpotLightUniforms(objectShader, spotLight);
	setPointLightUniforms(objectShader, pointLight);

	glUniform1i(objectShader.isObjectSunLocation, isObjectSun);
	glUniform1i(objectShader.fogEnabledLocation, FOG_ENABLED);

	glPolygonMode(GL_FRONT_AND_BACK, (wireframed) ? GL_LINE : GL_FILL);

	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Renderer::renderObject(ObjectType objectType, const Material material,
	const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
	bool wireframed, bool movingTexture, bool isObjectSun)
{
	if (objectType == OBJECT_TYPE_SIZE)
		return;

	if (objectType == MONKEY)
	{
		renderSpaceMonkey(material, modelMatrix, viewMatrix, projectionMatrix);
		return;
	}
	glUseProgram(objectShader.program);

	const MeshGeometry* mesh = geometry[objectType];

	setTransformUniforms(objectShader, 
		modelMatrix, 
		viewMatrix, 
		projectionMatrix, 
		movingTexture
	);

	setMaterialUniforms(objectShader, material);
	setDirectionalLightUniforms(objectShader, directionalLight);
	setSpotLightUniforms(objectShader, spotLight);
	setPointLightUniforms(objectShader, pointLight);

	glUniform1i(objectShader.activeLightningLocation, static_cast<int>(activeLightning));
	glUniform1i(objectShader.isObjectSunLocation, isObjectSun);
	glUniform1i(objectShader.fogEnabledLocation, FOG_ENABLED);

	
	glPolygonMode(GL_FRONT_AND_BACK,(wireframed)? GL_LINE: GL_FILL);


	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Renderer::renderSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glUseProgram(skyboxShader.program);

	glm::mat4 rotationMatrix = {
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};

	glm::mat4 inverseMatrix = glm::inverse(projectionMatrix * rotationMatrix);

	glUniformMatrix4fv(skyboxShader.inversePVMatrixLocation, 1, GL_FALSE, glm::value_ptr(inverseMatrix));
	glUniform1i(skyboxShader.skyboxSamplerLocation, 0);
	glUniform1i(skyboxShader.fogEnabledLocation, FOG_ENABLED);


	glBindVertexArray(skyBox->vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->material.texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, skyBox->numTriangles + 2);

	glBindVertexArray(0);
	glUseProgram(0);
}