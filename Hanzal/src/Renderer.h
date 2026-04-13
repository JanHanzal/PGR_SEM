#include "Mesh.h"
#include "Shader.h"

#ifndef __RENDERER_H
#define __RENDERER_H


class Renderer
{
	public:
		enum ObjectType
		{
			SPHERE,
			GHOUL,
			OBJECT_TYPE_SIZE
		};
		
	private:
		std::vector<MeshGeometry*> geometry;
		SCommonShaderProgram objectShader;

		DirectionalLight directionalLight;
		SpotLight spotLight;

//=============================================================
	private:
		void shadersInit();

		void loadMeshes();

		void setUpLights();

	public:
		~Renderer();

		void Init();


		void changeDirectionalLightParams(DirectionalLight newDirLight);

		void changeSpotLightParams(SpotLight _spotLight);
		
		void renderObject(ObjectType objectType,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
			const glm::mat4& projectionMatrix, bool wireframed);

		void renderObject(ObjectType objectType, const Material material,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
			const glm::mat4& projectionMatrix, bool wireframed);
};

#endif