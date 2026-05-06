#include "Renderer.h"

void Renderer::shadersInit()
{
	initializeObjectShaderProgram(objectShader);
	initializeSkyboxShaderProgram(skyboxShader);
	initializeAnimatedBillboardShaderProgram(animBillboardShader);
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
		glm::vec3(0.2f)				// Specular
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

		2500.0f,					// Radius
		false						// Is fading quadratic?
	};
}

Renderer::~Renderer()
{
	for (MeshGeometry* mesh : geometry)
	{
		cleanupGeometry(mesh);
	}
	pgr::deleteProgramAndShaders(objectShader.program);
	pgr::deleteProgramAndShaders(skyboxShader.program);
	pgr::deleteProgramAndShaders(animBillboardShader.program);
}

void Renderer::Init()
{
	shadersInit();
	setUpLights();
	loadMeshes();
	initHrouda();
	initFireBillboard(&geometry[FIRE]);
	initSkybox();
}

void Renderer::initHrouda()
{
	glGenVertexArrays(1, &hrouda.vao);
	glBindVertexArray(hrouda.vao);

	glGenBuffers(1, &hrouda.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, hrouda.vbo);

	glBufferData(GL_ARRAY_BUFFER, hrouda_n_vertices * hrouda_n_attribs_per_vertex * sizeof(float), hrouda_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &hrouda.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hrouda.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, hrouda_n_triangles * 3 * sizeof(unsigned int), hrouda_triangles, GL_STATIC_DRAW);
	

	glEnableVertexAttribArray(objectShader.posLocation);
	glVertexAttribPointer(objectShader.posLocation, 3, GL_FLOAT, GL_FALSE, hrouda_n_attribs_per_vertex * sizeof(float), (void*)0);

	glEnableVertexAttribArray(objectShader.normalLocation);
	glVertexAttribPointer(objectShader.normalLocation, 3, GL_FLOAT, GL_FALSE, hrouda_n_attribs_per_vertex * sizeof(float), (void*)(3 * sizeof(float)));
	

	glEnableVertexAttribArray(objectShader.texCoordLocation);
	glVertexAttribPointer(objectShader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, hrouda_n_attribs_per_vertex * sizeof(float), (void*)(6 * sizeof(float)));
	

	glBindVertexArray(0);
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
		std::string path = std::string("Data/Textures/Skybox/") + names[i] + ".jpg";
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

void Renderer::initFireBillboard(MeshGeometry** fireGeometry)
{
	const char* FIRE_TEXTURE_NAME = "Data/Textures/AnimatedBillboard/explosion.png";
	*fireGeometry = new MeshGeometry;

	(*fireGeometry)->material.texture = pgr::createTexture(FIRE_TEXTURE_NAME);

	glGenVertexArrays(1, &((*fireGeometry)->vertexArrayObject));
	glBindVertexArray((*fireGeometry)->vertexArrayObject);

	glGenBuffers(1, &((*fireGeometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (*fireGeometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(explosionVertexData), explosionVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(animBillboardShader.posLocation);

	glVertexAttribPointer(animBillboardShader.posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(animBillboardShader.texCoordLocation);
	glVertexAttribPointer(animBillboardShader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	(*fireGeometry)->numTriangles = explosionNumQuadVertices;

};


void Renderer::changeActiveLightning()//ActiveLightning _activeLightning)
{
	activeLightning = static_cast<ActiveLightning>((activeLightning + 1) % 3);
	//activeLightning = _activeLightning;
}

void Renderer::changeFogEnablement()
{
	FOG_ENABLED = !FOG_ENABLED;
}

void Renderer::toggleDirectionalLightParams()
{
	if (directionalLight.ambient == glm::vec3(0.1f))
	{
		directionalLight.ambient = glm::vec3(1.0f);
		directionalLight.diffuse = glm::vec3(0.0f);
		directionalLight.specular = glm::vec3(0.0f);
	}
	else
	{
		directionalLight.ambient = glm::vec3(0.1f);
		directionalLight.diffuse = glm::vec3(1.0f);
		directionalLight.specular = glm::vec3(0.2f);
	}
}

void Renderer::changeSpotLightParams(SpotLight _spotLight)
{
	spotLight = _spotLight;
}

void Renderer::changePointLightParams(PointLight _pointLight)
{
	pointLight = _pointLight;
}


void Renderer::renderHrouda(Material material, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glUseProgram(objectShader.program);
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

	glUniform1i(objectShader.isObjectSunLocation, false);
	glUniform1i(objectShader.fogEnabledLocation, FOG_ENABLED);

	
	glBindVertexArray(hrouda.vao);
	glDrawElements(GL_TRIANGLES, hrouda_n_triangles * 3, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	glUseProgram(0);

	CHECK_GL_ERROR();
}

void Renderer::renderObject(ObjectType objectType,
	const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix,
	bool wireframed, bool movingTexture, bool isObjectSun)
{
	if (objectType == OBJECT_TYPE_SIZE || objectType == FIRE)
		return;
	
	if (objectType == HROUDA)
	{
		Material mat = {
			glm::vec3(1.0f, 1.0f, 1.0f),  // Ambient
			glm::vec3(1.0f, 1.0f, 1.0f),  // Diffuse
		    glm::vec3(1.0f),              // Specular
			0.0f,                         // Shininess
			NULL
		};

		renderHrouda(mat, modelMatrix, viewMatrix, projectionMatrix);
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
	CHECK_GL_ERROR();
}

void Renderer::renderObject(ObjectType objectType, const Material material,
	const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
	bool wireframed, bool movingTexture, bool isObjectSun)
{
	if (objectType == OBJECT_TYPE_SIZE || objectType == FIRE)
		return;

	if (objectType == HROUDA)
	{
		renderHrouda(material, modelMatrix, viewMatrix, projectionMatrix);
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
	CHECK_GL_ERROR();
}

void Renderer::renderFire(glm::vec3 position, float size, float startTime, float currentTime, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	if (startTime > currentTime)
		return;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	glUseProgram(animBillboardShader.program);

	glm::mat4 billboardRotationMatrix = glm::mat4(
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, glm::vec3(size));

	matrix = matrix * billboardRotationMatrix;
	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

	

	glUniformMatrix4fv(animBillboardShader.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));
	glUniform1f(animBillboardShader.timeLocation, currentTime - startTime);
	glUniform1i(animBillboardShader.texSamplerLocation, 0);
	

	glBindVertexArray(geometry[FIRE]->vertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, geometry[FIRE]->material.texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry[FIRE]->numTriangles);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	
	CHECK_GL_ERROR();
}