#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "pgr.h"

#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f

#define WORLD_BOUND 2000


#define CAMERA_START_POSITION glm::vec3(100.0f, 0.0f, 20.0f)
#define CAMERA_START_ROTATION glm::vec3(0.0, 0.0f, 0.0f)
#define CAMERA_NEAR  0.01f
#define CAMERA_FAR  2000.0f

#define FPS 60
#define ANIMATION_LENGTH 0.1 *  24 // One frame length * number of frames



#define G 10 // Gravitational constant
#define ELASTIC_CONSTANT 0.6 // Energy loss on bounce 

#define SUN_POSITION glm::vec3(0.0)
#define SUN_ROTATION glm::vec3(90.0f, 0.0f, 0.0f)
#define SUN_SCALE glm::vec3(10.0)
#define SUN_MASS 1000000.0

#define EARTH_POSITION glm::vec3(-500.0f, 0.0f, 0.0f)
#define EARTH_ROTATION glm::vec3(90.0f, 0.0f, 0.0f)
#define EARTH_SCALE glm::vec3(2.0f)
#define EARTH_MASS 500.0

#define SATURN_POSITION glm::vec3(500.0f, 0.0f, 0.0f)
#define SATURN_ROTATION glm::vec3(90.0f, 0.0f, 0.0f)
#define SATURN_SCALE glm::vec3(5.0f)
#define SATURN_MASS 10000.0

//This enum is only used for initial order with only 3 planets -> might add more
enum PlanetsOrder
{
	SUN,
	EARTH,
	SATURN
};

#endif
