#include <iostream>
#include "pgr_sem.h"
#include "Mesh.h"
#include "Shader.h"

#ifndef __OBJECT_H
#define __OBJECT_H

#define G 10

class Object
{
    protected:
        Transform transform;
        Renderer::ObjectType objectType;

        glm::vec3 velocity;
        const char* sun = "Data/Earth.jpg";
        GLuint tex_;

    public:
        Object(Transform _transform, Renderer::ObjectType _objectType);

        virtual void Update(float deltaTime);

        void Move(glm::vec3 motion);

        void Rotate(glm::vec3 rotation);

        glm::vec3 getPosition();

        void renderObject(SCommonShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
};

class Planet : public Object
{
    private:
        float mass;
        const std::vector<Planet *> orbitPlanet;
        float speed = 10.0f;

    public:
        Planet(Transform _transform, Renderer::ObjectType _objectType, float _mass, const std::vector<Planet*>& _orbitPlanet);

        void Update(float timeDelta);

        float getAcceleration(const int mass, const float radius);

        float getMass();
};

#endif