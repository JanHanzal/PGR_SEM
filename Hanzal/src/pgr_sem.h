#include "pgr.h"

#ifndef __PGR_SEM_H
#define __PGR_SEM_H

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Camera
{
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
};


#endif