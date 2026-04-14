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
			MONKEY,
			OBJECT_TYPE_SIZE
		};

		enum ActiveLightning
		{
			DIRECTIONAL,
			SPOT,
			POINT
		};
		
	private:

		struct SpaceMonkey
		{
			GLuint vao = 0, vbo = 0, ebo = 0;
			GLuint monkey_texture;
		} spaceMonkey;
		

		std::vector<MeshGeometry*> geometry;
		SCommonShaderProgram objectShader;

		DirectionalLight directionalLight;
		SpotLight spotLight;
		PointLight pointLight;

		ActiveLightning activeLightning = DIRECTIONAL;
//=============================================================
	private:
		void shadersInit();

		void loadMeshes();

		void setUpLights();

		void initSpaceMonkey();

		void renderSpaceMonkey(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
			const glm::mat4& projectionMatrix);

	public:
		~Renderer();

		void Init();


		void changeDirectionalLightParams(DirectionalLight newDirLight);

		void changeSpotLightParams(SpotLight _spotLight);
		
		void changePointLightParams(PointLight _pointLight);

		void changeActiveLightning();//ActiveLightning _activeLightning);

		void renderObject(ObjectType objectType,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
			const glm::mat4& projectionMatrix, bool wireframed);

		void renderObject(ObjectType objectType, const Material material,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
			const glm::mat4& projectionMatrix, bool wireframed);
};

#endif