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
 
    float limit = WORLD_BOUND / 2.0;
    glm::vec3 newPosition = transform.position + motion;

    if (abs(newPosition.x) > limit)
    {
        motion.x = -motion.x;
        velocity.x = -velocity.x;
        velocity *= ELASTIC_CONSTANT;
    }
    if (abs(newPosition.y) > limit)
    {
        motion.y = -motion.y;
        velocity.y = -velocity.y * ELASTIC_CONSTANT;
        velocity *= ELASTIC_CONSTANT;
    }
    if (abs(newPosition.z) > limit)
    {
        motion.z = -motion.z;
        velocity.z = -velocity.z * ELASTIC_CONSTANT;
        velocity *= ELASTIC_CONSTANT;
    }

    
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

Transform Object::getTransform()
{
    return transform;
}

void Object::renderObject(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.scale));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), { 1.0f, 0.0f, 0.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), { 0.0f, 0.0f, 1.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), { 0.0f, 1.0f, 0.0f });
    
    bool wireframed = false;
    bool movingTexture = false;
    bool isObjectSun = false;

    renderer.renderObject(objectType, material, modelMatrix, viewMatrix, projectionMatrix, wireframed, movingTexture, isObjectSun);
}


Planet::Planet(Transform _transform, Renderer::ObjectType _objectType, Material material, float _mass, float _speed, bool _isSun, const std::vector<Planet*>& _planets) :
    Object(_transform, _objectType, material), mass(_mass), speed(_speed), isSun(_isSun), planets(_planets)
{
    velocity = glm::cross(glm::vec3(WORLD_UP_VECTOR), transform.position);
    if(glm::length(velocity) > 0)
        velocity = glm::normalize(velocity) * speed;
}


void Planet::Update(float timeDelta)
{
    if (isSun)
        return;
    
    for (Planet* planet : planets)
    {
        if (planet == this)
            continue;

        glm::vec3 dir = planet->getPosition() - transform.position;
        float radius = glm::length(dir);
        float acceleration = getAcceleration(planet->getMass(), radius);
        glm::vec3 accelerationVector = glm::normalize(dir) * acceleration;
        velocity += accelerationVector * timeDelta;
    }
    
    Move(velocity * timeDelta);

    glm::vec3 rotationVector = glm::vec3(0.0f, 30.0f, 0.0f);
    Rotate(rotationVector * timeDelta);
}

void Planet::renderObject(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(transform.scale));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), { 1.0f, 0.0f, 0.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), { 0.0f, 0.0f, 1.0f });
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), { 0.0f, 1.0f, 0.0f });

    bool wireframed = false;
    bool movingTexture = isSun;

    renderer.renderObject(objectType, material, modelMatrix, viewMatrix, projectionMatrix, wireframed, movingTexture, isSun);
}

float Planet::getAcceleration(const float mass, const float radius)
{
    if (radius <= transform.scale.x)
        return G * mass;
    float acceleration = G * mass / pow(radius, 2);
    return acceleration;
}

float Planet::getMass()
{
    return mass;
}


float Fire::getStartingTime()
{
    return started;
}

Transform Fire::getTransform()
{
    return transform;
}
