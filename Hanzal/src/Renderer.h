#include "Mesh.h"
#include "Shader.h"
#include <unordered_map>
#include <iostream>

#ifndef __RENDERER_H
#define __RENDERER_H


class Renderer
{
	private:
		std::unordered_map<int, MeshGeometry*> geometry;
		SCommonShaderProgram objectShader;

		DirectionalLight directionalLight;

	public:
		enum ObjectType
		{
			Sphere, 
			ObjectTypeSize
		};


		//Methods
	private:
		void shadersInit()
		{
			initializeShaderPrograms(objectShader);
		}

		void loadMeshes()
		{
			const char* sphere = "Data/Sphere.obj";
			const char* earth = "Data/Earth.obj";
			const char* qhoul = "Data/ghoul.obj";
			loadSingleMesh(sphere, objectShader, &geometry[Sphere]);
			CHECK_GL_ERROR();
		}

		void setUpLights()
		{
			directionalLight = {
			   glm::vec3(0.0f, 0.0f, 1.0f), // Position
			   glm::vec3(0.5f),				// Ambient
			   glm::vec3(1.0f),				// Diffuse
			   glm::vec3(0.1f)				// Specular
			};
		}
	public:
		~Renderer()
		{
			for (auto it = geometry.begin(); it != geometry.end();)
			{
				cleanupGeometry(it->second);
			}
			cleanupShaderPrograms(objectShader);
		}

		void Init()
		{
			shadersInit();
			loadMeshes();
		}



		void changeDirectionalLightParams(DirectionalLight newDirLight)
		{
			directionalLight = newDirLight;
		}
		
		void renderObject(ObjectType objectType, const Material material,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, 
			const glm::mat4& projectionMatrix, bool wireframed)
		{
			const MeshGeometry* mesh = geometry[objectType];

			glUseProgram(objectShader.program);
			
			if(wireframed)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


			setTransformUniforms(
				objectShader, 
				modelMatrix, 
				viewMatrix, 
				projectionMatrix
			);

			setMaterialUniforms(objectShader, material);
			setDirectionalLightUniforms(objectShader, directionalLight);

			glBindVertexArray(mesh->vertexArrayObject);
			glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
			glUseProgram(0);
		}
};

#endif