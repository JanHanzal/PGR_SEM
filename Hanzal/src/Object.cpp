#include "Object.h"


Object::Object(Transform _transform, Renderer::ObjectType _objectType, Material _material) :
    transform(_transform), objectType(_objectType), material(_material)
{};

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

void Object::renderObject(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.scale));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), { 1.0f, 0.0f, 0.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), { 0.0f, 0.0f, 1.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), { 0.0f, 1.0f, 0.0f });
    
    renderer.renderObject(objectType, material, modelMatrix, viewMatrix, projectionMatrix, false);
}


Planet::Planet(Transform _transform, Renderer::ObjectType _objectType, Material material, float _mass, float _speed, const std::vector<Planet*>& _orbitPlanet) :
    Object(_transform, _objectType, material), mass(_mass), orbitPlanet(_orbitPlanet), speed(_speed)
{
    velocity = glm::cross(glm::vec3(WORLD_UP_VECTOR), transform.position);
    if(glm::length(velocity) > 0)
        velocity = glm::normalize(velocity) * speed;
}

void Planet::Update(float timeDelta)
{
    for (Planet* p : orbitPlanet)
    {
        glm::vec3 dir = p->getPosition() - transform.position;
        float radius = glm::length(dir);
        float acceleration = getAcceleration(p->getMass(), radius);
        glm::vec3 accelerationVector = glm::normalize(dir) * acceleration;
        velocity += accelerationVector * timeDelta;
    }

    Move(velocity * timeDelta);

    glm::vec3 rotationVector = glm::vec3(0.0f, 30.0f, 0.0f);
    Rotate(rotationVector * timeDelta);
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