#include "Object.h"


Object::Object(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader) : transform(_transform)
{
    tex_ = pgr::createTexture(sun);
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

    
    Material mat = {
       glm::vec3(1.0),
       glm::vec3(1.0),
       glm::vec3(0.0),
       0.0f,
       tex_
    };

    
    setMaterialUniforms(shaderProgram, mat);

    glBindVertexArray(mesh->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

MeshGeometry* Object::getMesh()
{
    return mesh;
}



Planet::Planet(Transform _transform, const char* mesh_file_location, SCommonShaderProgram& shader, float _mass, const std::vector<Planet*>& _orbitPlanet) :
    Object(_transform, mesh_file_location, shader), mass(_mass), orbitPlanet(_orbitPlanet)
{
    velocity = glm::cross(glm::vec3(WORLD_UP_VECTOR), transform.position);
    if(glm::length(velocity) > 0)
        velocity = glm::normalize(velocity) * speed;
}

Planet:: ~Planet()
{
    cleanupGeometry(mesh);
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