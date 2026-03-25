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
    glm::vec3 direction;
    MeshGeometry* mesh = NULL;

  public:
    Object(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader)
    {
        if (!loadSingleMesh(mesh_file_location, shader, &mesh))
            std::cout << "ERROR: Failed to load mesh: " << mesh_file_location << std::endl;
        CHECK_GL_ERROR();
    };

    ~Object()
    {
        cleanupGeometry(mesh);
    }
    void Update(float deltaTime)
    {
        glm::vec3 motion = direction * deltaTime;
        Move(motion);
    }

    void Move(glm::vec3 motion)
    {
        transform.position += motion;
    }


    glm::vec3 getPosition()
    {
        return transform.position;
    }

    void renderObject(SCommonShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
    {
        glUseProgram(shaderProgram.program);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.scale));

        setTransformUniforms(&shaderProgram, modelMatrix, viewMatrix, projectionMatrix);

        glBindVertexArray(mesh->vertexArrayObject);
        glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        return;
    }
};



class Planet : Object
{
  private:
    float mass;
    Planet* orbitPlanet = nullptr;


  public:
    Planet(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader, float _mass, Planet* _orbitPlanet) : 
            Object(_transform, mesh_file_location, shader),
            mass(_mass), 
            orbitPlanet(_orbitPlanet)
    {


    };

    void Update(float timeDelta)
    {
        if (orbitPlanet == nullptr) 
            return;
      
        glm::vec3 dir = orbitPlanet->getPosition() - transform.position;
        float radius = glm::length(dir);

        if (radius == 0.0f) 
            return;

        float acceleration = getAcceleration(orbitPlanet->getMass(), radius);

        glm::vec3 accelerationVector = glm::normalize(dir) * acceleration;
        
        direction += accelerationVector * timeDelta;
        Move(direction * timeDelta);
    }

    float getAcceleration(const int mass, const float radius)
    {
        float acceleration = G * mass / pow(radius, 2);
        return acceleration;
    }

    float getMass()
    {
        return mass;
    }
    
};


#endif __OBJECT_H