#include "Object.h"


Object::Object(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader) : transform(_transform)
{
    if (!loadSingleMesh(mesh_file_location, shader, &mesh))
        std::cout << "ERROR: Failed to load mesh: " << mesh_file_location << std::endl;
    CHECK_GL_ERROR();
}

Object::~Object()
{
    cleanupGeometry(mesh);
}

void Object::Update(float deltaTime)
{
    glm::vec3 motion = velocity * deltaTime;
    Move(motion);
    transform.rotation.z += 30.0f * deltaTime;
}

void Object::Move(glm::vec3 motion)
{
    transform.position += motion;
}

void Object::Rotate(glm::vec3 rotation)
{
    transform.rotation += rotation;
}

glm::vec3 Object::getPosition()
{
    return transform.position;
}

void Object::renderObject(SCommonShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glUseProgram(shaderProgram.program);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.scale));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), { 1.0f, 0.0f, 0.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), { 0.0f, 0.0f, 1.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), { 0.0f, 1.0f, 0.0f });

    setTransformUniforms(
        shaderProgram,
        modelMatrix,
        viewMatrix,
        projectionMatrix
    );

    setMaterialUniforms(
        shaderProgram,
        mesh->ambient,
        mesh->diffuse,
        mesh->specular,
        mesh->shininess,
        mesh->texture
    );

    glBindVertexArray(mesh->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}




Planet::Planet(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader, float _mass, Planet* _orbitPlanet) :
    Object(_transform, mesh_file_location, shader),
    mass(_mass),
    orbitPlanet(_orbitPlanet)
{
    if (_orbitPlanet != nullptr)
        velocity = glm::cross(glm::vec3(WORLD_UP_VECTOR), orbitPlanet->getPosition() - transform.position);
    velocity = glm::normalize(velocity) * speed;
}

void Planet::Update(float timeDelta)
{
    glm::vec3 rotationVector = glm::vec3(0.0f, 30.0f, 0.0f);
    Rotate(rotationVector * timeDelta);

    if (orbitPlanet == nullptr)
        return;

    glm::vec3 dir = orbitPlanet->getPosition() - transform.position;
    float radius = glm::length(dir);

    if (radius == 0.0f)
        return;

    float acceleration = getAcceleration(orbitPlanet->getMass(), radius);

    glm::vec3 accelerationVector = glm::normalize(dir) * acceleration;

    velocity += accelerationVector * timeDelta;

    Move(velocity * timeDelta);
}

float Planet::getAcceleration(const int mass, const float radius)
{
    float acceleration = G * mass / pow(radius, 2);
    return acceleration;
}

float Planet::getMass()
{
    return mass;
}