#ifndef __SPLINE_H__
#define __SPLINE_H__


#include "pgr.h"

class Spline
{
private:
	glm::vec3 P0, P1, P2, P3;

public:
	Spline(glm::vec3 _P0, glm::vec3 _P1, glm::vec3 _P2, glm::vec3 _P3);
	glm::vec3 get_Position_Catmull_Rom(float t);

};


#endif
