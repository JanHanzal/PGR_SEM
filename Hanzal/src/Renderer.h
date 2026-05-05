#ifndef __RENDERER_H
#define __RENDERER_H

#include "Mesh.h"
#include "Shader.h"
#include "C_meshData.h"


class Renderer
{
	public:
		enum ObjectType
		{
			SPHERE,
			GHOUL,
			HROUDA,
			FIRE,
			OBJECT_TYPE_SIZE
		};

		enum ActiveLightning
		{
			DIRECTIONAL,
			SPOT,
			POINT
		};
		
		struct hrouda
		{
			GLuint vao = 0, vbo = 0, ebo = 0;
			

			glm::vec3 position;
			glm::vec3 direction;

		} hrouda;

	private:

		std::vector<MeshGeometry*> geometry;
		MeshGeometry* skyBox = nullptr;

		ObjectShader objectShader;
		SkyboxShader skyboxShader;
		AnimatedBillboardShaderProgram animBillboardShader;


		DirectionalLight directionalLight;
		SpotLight spotLight;
		PointLight pointLight;

		ActiveLightning activeLightning = DIRECTIONAL;

		bool FOG_ENABLED = false;
//=============================================================
	private:
		void shadersInit();

		void loadMeshes();

		void setUpLights();

		void initHrouda();

		void renderHrouda(Material material, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		void initSkybox();

		void initFireBillboard(MeshGeometry** fireGeometry);

	public:
		~Renderer();

		void Init();

		void toggleDirectionalLightParams();

		void changeSpotLightParams(SpotLight _spotLight);
		
		void changePointLightParams(PointLight _pointLight);

		void changeActiveLightning();


		void changeFogEnablement();

		void renderObject(ObjectType objectType,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
			bool wireframed, bool movingTexture, bool isObjectSun);

		void renderObject(ObjectType objectType, const Material material,
			const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
			bool wireframed, bool movingTexture, bool isObjectSun);

		void renderSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

		void renderFire(glm::vec3 position, float size,float startTime, float currentTime, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

};

#endif