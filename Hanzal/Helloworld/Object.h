#include "pgr.h"
#include "pgr_sem.h"
 

#define G 10

class Object
{
  protected:
    Transform transform;
    glm::vec3 direction;

  public:
      Object() {};
    Object(Transform _transform): transform(_transform) {};
    
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
};



class Planet : Object
{
  private:
    float mass;
    Planet* orbitPlanet = nullptr;


  public:
    Planet(Transform _transform, float _mass) : Object(_transform), mass(_mass) {};

    Planet(Transform _transform, float _mass, Planet* _orbitPlanet) : Object(_transform), 
                                                                      mass(_mass), 
                                                                      orbitPlanet(_orbitPlanet) {};

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