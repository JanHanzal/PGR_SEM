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
        glm::vec3 velocity;
        MeshGeometry* mesh = NULL;
        const char* sun = "Data/Earth.jpg";
        GLuint tex_;

    public:
        Object(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader);

        ~Object();

        virtual void Update(float deltaTime);

        void Move(glm::vec3 motion);

        void Rotate(glm::vec3 rotation);

        glm::vec3 getPosition();

        void renderObject(SCommonShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

        MeshGeometry* getMesh();
};

class Planet : public Object
{
    private:
        float mass;
        const std::vector<Planet *> orbitPlanet;
        float speed = 10.0f;

    public:
        Planet(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader, float _mass, const std::vector<Planet*>& _orbitPlanet);

        ~Planet();

        void Update(float timeDelta);

        float getAcceleration(const int mass, const float radius);

        float getMass();
};

#endif