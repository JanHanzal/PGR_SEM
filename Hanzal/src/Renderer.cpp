#include "Renderer.h"

void Renderer::shadersInit()
{
	initializeShaderPrograms(objectShader);
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
}

void Renderer::changeActiveLightning()//ActiveLightning _activeLightning)
{
	activeLightning = static_cast<ActiveLightning>((activeLightning + 1) % 3);
	//activeLightning = _activeLightning;
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


void Renderer::renderObject(ObjectType objectType,
	const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
	const glm::mat4& projectionMatrix, bool wireframed)
{
	if (objectType == OBJECT_TYPE_SIZE)
		return;

 	const MeshGeometry* mesh = geometry[objectType];

	glUseProgram(objectShader.program);

	if (wireframed)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	setTransformUniforms(
		objectShader,
		modelMatrix,
		viewMatrix,
		projectionMatrix
	);

	setMaterialUniforms(objectShader, mesh->material);
	setDirectionalLightUniforms(objectShader, directionalLight);

	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Renderer::renderObject(ObjectType objectType, const Material material,
	const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
	const glm::mat4& projectionMatrix, bool wireframed)
{
	if (objectType == OBJECT_TYPE_SIZE)
		return;

	const MeshGeometry* mesh = geometry[objectType];

	glUseProgram(objectShader.program);

	if (wireframed)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	setTransformUniforms(objectShader, modelMatrix, viewMatrix, projectionMatrix);
	setMaterialUniforms(objectShader, material);

	setDirectionalLightUniforms(objectShader, directionalLight);
	setSpotLightUniforms(objectShader, spotLight);
	setPointLightUniforms(objectShader, pointLight);
	
	glUniform1i(objectShader.activeLightningLocation, static_cast<int>(activeLightning));

	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}