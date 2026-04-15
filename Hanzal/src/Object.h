#ifndef __OBJECT_H
#define __OBJECT_H

#include <iostream>
#include "pgr_sem.h"
#include "Mesh.h"
#include "Shader.h"
#include "Configuration.h"


class Object
{
    protected:
        Transform transform;
        Renderer::ObjectType objectType;
        Material material;

        glm::vec3 velocity;

    public:
        Object(Transform _transform, Renderer::ObjectType _objectType, Material _material);

        virtual void Update(float deltaTime);

        void Move(glm::vec3 motion);

        void Rotate(glm::vec3 rotation);

        glm::vec3 getPosition();

        void renderObject(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
};

class Planet : public Object
{
    private:
        float mass;
        const std::vector<Planet *>& planets;
        float speed = 20.0f;
        bool isSun;

    public:
        Planet(Transform _transform, Renderer::ObjectType _objectType, Material material, float _mass, float _speed, bool _isSun, const std::vector<Planet*>& _planets);
        
        void Update(float timeDelta);

        float getAcceleration(const float mass, const float radius);

        float getMass();
};

#endif